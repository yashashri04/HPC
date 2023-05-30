#include <iostream>
#include <stack>
#include <omp.h>

struct Node
{
    int data;
    Node *left;
    Node *right;
};

void parallelDFS(Node *root)
{
    if (root == nullptr)
        return;

    std::stack<Node *> stack;
    stack.push(root);

#pragma omp parallel
    {
        while (!stack.empty())
        {
#pragma omp for nowait
            for (int i = 0; i < stack.size(); ++i)
            {
                Node *current = nullptr;

#pragma omp critical
                {
                    current = stack.top();
                    stack.pop();
                }

                // Process current node
                std::cout << current->data << " ";

                // Push child nodes
                if (current->right != nullptr)
                {
#pragma omp critical
                    {
                        stack.push(current->right);
                    }
                }

                if (current->left != nullptr)
                {
#pragma omp critical
                    {
                        stack.push(current->left);
                    }
                }
            }
        }
    }
}

int main()
{
    Node *root = new Node{1, nullptr, nullptr};
    root->left = new Node{2, nullptr, nullptr};
    root->right = new Node{3, nullptr, nullptr};
    root->left->left = new Node{4, nullptr, nullptr};
    root->left->right = new Node{5, nullptr, nullptr};

    parallelDFS(root);

    return 0;
}
