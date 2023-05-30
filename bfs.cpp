#include <iostream>
#include <queue>
#include <omp.h>

struct Node
{
    int data;
    Node *left;
    Node *right;
};

void parallelBFS(Node *root)
{
    if (root == nullptr)
        return;

    std::queue<Node *> q;
    q.push(root);

#pragma omp parallel
    {
        while (!q.empty())
        {
#pragma omp for
            for (int i = 0; i < q.size(); ++i)
            {
                Node *current = nullptr;

#pragma omp critical
                {
                    current = q.front();
                    q.pop();
                }

                // Process current node
                std::cout << current->data << " ";

                // Enqueue child nodes
                if (current->left != nullptr)
                {
#pragma omp critical
                    {
                        q.push(current->left);
                    }
                }

                if (current->right != nullptr)
                {
#pragma omp critical
                    {
                        q.push(current->right);
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

    parallelBFS(root);

    return 0;
}
