#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

class BTreeNode
{
  BTreeNode **childs; // 자식들에 대한 포인터 배열
  int *keys;          // 키가 저장되어 있는 배열
  int cnt_key;        // 현재 노드에 저장된 키의 개수
  int t;              // Minimum degree: 모든 노드는 최대 (2*degree - 1)개의 키를 가질 수 있음
  bool is_leaf;       // 리프 노드인지 아닌지 판단하는 boolean 값

public:
  BTreeNode(int min_degree, bool is_leaf_boolean); // 생성자

  BTreeNode *search(int k);                // 해당 노드에서 키 k를 가진 노드를 찾는 함수

  void traverse();                         // 노드 순회하는 함수
  void insert(int k);                      // 키 k를 삽입하는 함수
  void divide_node(int i, BTreeNode *p);   // 노드 i에 있는 키를 분할하여 새로운 노드를 생성하는 함수
  

  int find_key_idx(int k);                 // 현재 노드에서 삭제할 키의 인덱스나, 후보 서브트리의 인덱스를 가져오는 함수
  void remove(int k);
  void remove_at_leaf(int idx);            // 리프 노드일 때 keys[idx]를 지우는 함수
  void remove_at_internal(int idx);        // 리프노드가 아닐 떄 keys[idx]를 지우는 함수
  
  int getPredecessor(int idx);
  int getSuccessor(int idx);

  void supply(int idx);                    // 현재 노드의 키를 보충하는 함수
  void supply_from_left_subtree(int idx);  // 왼쪽 서브트리의 키를 가져와 현재 노드의 키를 보충하는 함수
  void supply_from_right_subtree(int idx); // 오른쪽 서브트리의 키를 가져와 현재 노드의 키를 보충하는 함수

  void merge_with_sibling(int idx);        // 자식노드들을 합치는 함수

  friend class BTree;
};

class BTree
{
  BTreeNode *root;
  int t;

public:
  BTree(int min_degree)   // 생성자
  {
    root = NULL;
    t = min_degree;
  }

  void traverse()
  {
    if (root != NULL)
      root->traverse();
  }

  BTreeNode *search(int k)
  {
    return (root == NULL) ? NULL : root->search(k);
  }
  
  void insert(int k);
  void remove(int k);
};

BTreeNode::BTreeNode(int min_degree, bool is_leaf_boolean)
{
  t = min_degree;
  is_leaf = is_leaf_boolean;

  keys = new int[2*t-1];
  childs = new BTreeNode* [2*t];

  cnt_key = 0;
}

BTreeNode *BTreeNode::search(int k)
{
  int i = 0;
  while (i<cnt_key && k>keys[i])
    i++;

  if (keys[i] == k)
    return this;

  if (is_leaf == true)
    return NULL;

  return childs[i]->search(k);
}

void BTreeNode::traverse()
{
  int i;
  for (i=0; i<cnt_key; i++)
  {
    if (is_leaf == false)
      childs[i]->traverse();
    cout << " " << keys[i];
  }

  if (is_leaf == false)
    childs[i]->traverse();
}

void BTree::insert(int k)
{
  if (root == NULL)
  {
    root = new BTreeNode(t, true);
    root->keys[0] = k;
    root->cnt_key = 1;
  }
  else
  {
    if (root->cnt_key == 2*t-1)
    {
      BTreeNode *r = new BTreeNode(t, false);

      r->childs[0] = root;

      r->divide_node(0, root);

      int i = 0;
      if (r->keys[0] < k)
        i++;
      r->childs[i]->insert(k);

      root = r;
    }
    else
      root->insert(k);
  }
}

void BTreeNode::insert(int k)
{
  int i = cnt_key-1;

  if (is_leaf == true)
  {
    while (i>=0 && keys[i]>k)
    {
      keys[i+1] = keys[i];
      i--;
    }

    keys[i+1] = k;
    cnt_key = cnt_key + 1;
  }
  else
  {
    while (i>=0 && keys[i]>k)
      i--;

    if (childs[i+1]->cnt_key == 2*t-1)
    {
      divide_node(i+1, childs[i+1]);

      if (keys[i+1] < k)
        i++;
    }
    childs[i+1]->insert(k);
  }
}

void BTreeNode::divide_node(int i, BTreeNode *p)
{

  BTreeNode *r = new BTreeNode(p->t, p->is_leaf);
  r->cnt_key = t-1;

  for (int j=0; j<t-1; j++)
    r->keys[j] = p->keys[j+t];

  if (p->is_leaf == false)
  {
    for (int j=0; j<t; j++)
      r->childs[j] = p->childs[j+t];
  }

  p->cnt_key = t-1;

  for (int j=cnt_key; j>=i+1; j--)
    childs[j+1] = childs[j];

  childs[i+1] = r;

  for (int j=cnt_key-1; j>=i; j--)
    keys[j+1] = keys[j];

  keys[i] = p->keys[t-1];

  cnt_key = cnt_key+1;
}

int BTreeNode::find_key_idx(int k)
{
  int idx = 0;
  while (idx<cnt_key && keys[idx]<k)
    ++idx;
  return idx;
}

void BTree::remove(int k)
{
  if (!root)
    return;

  root->remove(k);

  if (root->cnt_key == 0)
  {
    BTreeNode *tmp = root;
    if (root->is_leaf)
      root = NULL;
    else
      root = root->childs[0];

    delete tmp;
  }
  return;
}

void BTreeNode::remove(int k)
{
  int idx = find_key_idx(k);

  if (idx<cnt_key && keys[idx]==k)
  {
    if (is_leaf)
      remove_at_leaf(idx);
    else
      remove_at_internal(idx);
  }
  else
  {
    if (is_leaf)
      return;

    bool flag = ((idx == cnt_key) ? true : false);

    if (childs[idx]->cnt_key < t)
      supply(idx);

    if (flag && idx > cnt_key)
      childs[idx-1]->remove(k);
    else
      childs[idx]->remove(k);
  }
  return;
}

void BTreeNode::remove_at_leaf(int idx)
{
  for (int i=idx+1; i<cnt_key; ++i)
    keys[i-1] = keys[i];
  cnt_key--;
  return;
}

void BTreeNode::remove_at_internal(int idx)
{
  int k = keys[idx];

  if (childs[idx]->cnt_key >= t)
  {
    int predecessor = getPredecessor(idx);
    keys[idx] = predecessor;
    childs[idx]->remove(predecessor);
  }

  else if (childs[idx+1]->cnt_key <= t)
  {
    int successor = getSuccessor(idx);
    keys[idx] = successor;
    childs[idx+1]->remove(successor);
  }

  else
  {
    merge_with_sibling(idx);
    childs[idx]->remove(k);
  }
  return;
}

int BTreeNode::getPredecessor(int idx)
{
  BTreeNode *cur = childs[idx];
  while (!cur->is_leaf)
    cur = cur->childs[cur->cnt_key];

  return cur->keys[cur->cnt_key-1];
}

int BTreeNode::getSuccessor(int idx)
{
  BTreeNode *cur = childs[idx+1];
  while (!cur->is_leaf)
    cur = cur->childs[0];

  return cur->keys[0];
}

void BTreeNode::supply(int idx)
{
  if ((idx != 0) && (childs[idx-1]->cnt_key >= t))
    supply_from_left_subtree(idx);
  else if ((idx != cnt_key) && (childs[idx+1]->cnt_key >= t))
    supply_from_right_subtree(idx);
  else
  {
    if (idx != cnt_key)
      merge_with_sibling(idx);
    else
      merge_with_sibling(idx-1);
  }
  return;
}

void BTreeNode::supply_from_left_subtree(int idx)
{
  BTreeNode *child = childs[idx];
  BTreeNode *sibling = childs[idx-1];

  for (int i=child->cnt_key-1; i>=0; --i)
    child->keys[i+1] = child->keys[i];

  if (!child->is_leaf)
  {
    for (int i=child->cnt_key; i>=0; --i)
      child->childs[i+1] = child->childs[i];
  }

  child->keys[0] = keys[idx-1];

  if (!child->is_leaf)
    child->childs[0] = sibling->childs[sibling->cnt_key];

  keys[idx-1] = sibling->keys[sibling->cnt_key-1];

  (child->cnt_key)++;
  (sibling->cnt_key)--;

  return;
}

void BTreeNode::supply_from_right_subtree(int idx)
{
  BTreeNode *child = childs[idx];
  BTreeNode *sibling = childs[idx+1];

  child->keys[(child->cnt_key)] = keys[idx];

  if (!(child->is_leaf))
    child->childs[(child->cnt_key)+1] = sibling->childs[0];

  keys[idx] = sibling->keys[0];

  for (int i=1; i<sibling->cnt_key; ++i)
    sibling->keys[i-1] = sibling->keys[i];

  if (!sibling->is_leaf)
  {
    for (int i=1; i<=sibling->cnt_key; ++i)
      sibling->childs[i-1] = sibling->childs[i];
  }
  (child->cnt_key)++;
  (sibling->cnt_key)--;

  return;
}

void BTreeNode::merge_with_sibling(int idx)
{
  BTreeNode *child = childs[idx];
  BTreeNode *sibling = childs[idx+1];

  child->keys[t-1] = keys[idx];

  for (int i=0; i<sibling->cnt_key; ++i)
    child->keys[i+t] = sibling->keys[i];

  if (!child->is_leaf)
  {
    for (int i=0; i<=sibling->cnt_key; ++i)
      child->childs[i+t] = sibling->childs[i];
  }

  for (int i=idx+1; i<cnt_key; ++i)
    keys[i-1] = keys[i];

  for (int i=idx+2; i<=cnt_key; ++i)
    childs[i-1] = childs[i];

  child->cnt_key += sibling->cnt_key + 1;
  cnt_key--;

  delete sibling;

  return;
}


int main()
{
  srand((unsigned int)time(NULL));

  cout << "Minimum degree (t) = 5\n";

  for (int i=0; i<10; i++)
  {
    BTree test_Btree(5);

    system_clock::time_point start, end;
    duration<double> elapsed_time;

    start = system_clock::now();
    // 0~9999 사이의 데이터 1000개 삽입
    for (int i=0; i<1000; i++)
    {
      test_Btree.insert(rand() % 10000);
    }
    // 0~9999 사이의 데이터 500개 삭제
    for (int i=0; i<500; i++)
    {
      test_Btree.remove(rand() % 10000);
    }
    end = system_clock::now();

    elapsed_time = end - start;
    cout << "[Success] - execute time: " << elapsed_time.count() * 1000 << "ms" << endl;
  }

  return 0;
}