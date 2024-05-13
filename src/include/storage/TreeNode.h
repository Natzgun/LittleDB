#ifndef TREENODE_H
#define TREENODE_H

#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

struct TreeNode {
  fs::path directory;
  std::vector<TreeNode> children;
};

#endif // TREENODE_H
