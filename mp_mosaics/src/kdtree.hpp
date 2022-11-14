/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
  if (first[curDim] == second[curDim]) return first < second;
  else return first[curDim] < second[curDim];
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
  double curr_dist = 0;
  double pot_dist = 0;
  for (int i = 0; i < Dim; i++) {
    curr_dist += (currentBest[i] - target[i]) * (currentBest[i] - target[i]);
    pot_dist += (potential[i] - target[i]) * (potential[i] - target[i]);
  }
  if (curr_dist == pot_dist) return potential < currentBest;
  return pot_dist < curr_dist;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    vector<Point<Dim>> points = newPoints;
    buildTreeHelper(points, 0, root);
}

template <int Dim>
void KDTree<Dim>::buildTreeHelper(vector<Point<Dim>>& points, int dim, KDTreeNode*& curr)
{
  if (points.empty()) return;

  int mid_idx = floor((points.size() - 1) / 2);
  Point<Dim> mid_point = quickselect(points, dim, mid_idx);

  curr = new KDTree<Dim>::KDTreeNode(mid_point);
  
  if (points.size() != 1) {
    vector<Point<Dim>> left;
    left.insert(left.end(), points.begin(), points.begin() + mid_idx);
    buildTreeHelper(left, (dim + 1) % Dim, curr->left);
  }
  if (points.size() != 1) {
    vector<Point<Dim>> right;
    right.insert(right.end(), points.begin() + mid_idx + 1, points.end());
    buildTreeHelper(right, (dim + 1) % Dim, curr->right);
  }
}

template <int Dim>
Point<Dim> KDTree<Dim>::quickselect(vector<Point<Dim>>& points, int dim, int k)
{
  int pivot_idx = points.size() - 1;
  pivot_idx = partition(points, dim);
  if (k == pivot_idx) return points[k];
  else if (k < pivot_idx) {
    vector<Point<Dim>> partitioned;
    partitioned.assign(points.begin(), points.begin() + pivot_idx);
    return quickselect(partitioned, dim, k);
  } else {
    vector<Point<Dim>> partitioned;
    partitioned.assign(points.begin() + pivot_idx, points.end());
    return quickselect(partitioned, dim, k - pivot_idx);
  }
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& points, int dim)
{
  if (points.size() == 1) return 0;

  Point<Dim> pivot = points.back();
  int i = 0;

  for (unsigned j = 0; j < points.size() - 1; j++) {
    if (smallerDimVal(points[j], pivot, dim)) {
      Point<Dim> tmp = points[i];
      points[i] = points[j];
      points[j] = tmp;
      i++;
    }
  }

  Point<Dim> tmp = points[i];
  points[i] = points[points.size() - 1];
  points[points.size() - 1] = tmp;
  return i;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  root = copy(other.root);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  if (!this == &rhs) {
    destroy(root);
    root = copy(rhs.root);
  }
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  destroy(root);
  root = nullptr;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode KDTree<Dim>::copy(const KDTreeNode* root) {
  if (root == NULL) return root;

  KDTreeNode node = new KDTreeNode(root);
  node->left = copy(root->left);
  node->left = copy(root->right);

  return node;
}

template <int Dim>
void KDTree<Dim>::destroy(KDTreeNode* node) {
  if (node == NULL) return;
  destroy(node->left);
  destroy(node->right);
  delete node;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
  return findNearestNeighborHelper(root, query, 0);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighborHelper(KDTreeNode* root, const Point<Dim>& query, int dim) const
{
  if (root == NULL) return Point<Dim>();
  else if (root->left == NULL && root->right == NULL) return root->point;

  Point<Dim> nearest = root->point;
  Point<Dim> poss_nearest;
  bool go_left = root->left != NULL && smallerDimVal(query, nearest, dim);

  if (go_left) poss_nearest = findNearestNeighborHelper(root->left, query, (dim + 1) % Dim);
  else poss_nearest = findNearestNeighborHelper(root->right, query, (dim + 1) % Dim);

  if (shouldReplace(query, nearest, poss_nearest)) nearest = poss_nearest;

  double r = 0.0;
  for (int i = 0; i < Dim; i++) r += ((nearest[i] - query[i]) * (nearest[i] - query[i]));
  r = sqrt(r);

  if (query[dim] - r <= root->point[dim] && query[dim] + r >= root->point[dim]) {
    if (go_left && root->right != NULL) poss_nearest = findNearestNeighborHelper(root->right, query, (dim + 1) % Dim);
    else if (root->left != NULL) poss_nearest = findNearestNeighborHelper(root->left, query, (dim + 1) % Dim);

    if (shouldReplace(query, nearest, poss_nearest)) nearest = poss_nearest;
  }

  return nearest;
}

