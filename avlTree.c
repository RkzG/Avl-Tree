#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Code By : Franky Wahyu Prasetyo / 2702507920
// Struktur node AVL
typedef struct Node {
    char name[50];
    int priority;
    int duration;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

//Code By : Franky Wahyu Prasetyo / 2702507920
int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Node* node) {
    return node ? node->height : 0;
}

int getBalance(Node* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

//Code By : Franky Wahyu Prasetyo / 2702507920
// Rotasi kanan
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Rotasi kiri
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

//Code By : Franky Wahyu Prasetyo / 2702507920
// Membuat node baru
Node* createNode(char* name, int priority, int duration) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->priority = priority;
    newNode->duration = duration;
    newNode->height = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Menambahkan building ke AVL Tree
Node* insertBuilding(Node* root, char* name, int priority, int duration) {
    if (!root) return createNode(name, priority, duration);

    if (duration < root->duration)
        root->left = insertBuilding(root->left, name, priority, duration);
    else if (duration > root->duration)
        root->right = insertBuilding(root->right, name, priority, duration);
    else
        return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && duration < root->left->duration)
        return rightRotate(root);

    if (balance < -1 && duration > root->right->duration)
        return leftRotate(root);

    if (balance > 1 && duration > root->left->duration) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && duration < root->right->duration) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
//Code By : Franky Wahyu Prasetyo / 2702507920
// Get Data AVL Tree
void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    printf("Name: %s | Priority: %d | Duration: %d days\n", root->name, root->priority, root->duration);
    inorder(root->right);
}
//Code By : Franky Wahyu Prasetyo / 2702507920
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

Node* finishBuilding(Node* root, int duration) {
    if (!root) return root;

    if (duration < root->duration)
        root->left = finishBuilding(root->left, duration);
    else if (duration > root->duration)
        root->right = finishBuilding(root->right, duration);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }

        Node* temp = minValueNode(root->right);
        strcpy(root->name, temp->name);
        root->priority = temp->priority;
        root->duration = temp->duration;
        root->right = finishBuilding(root->right, temp->duration);
    }

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

//Code By : Franky Wahyu Prasetyo / 2702507920
// Fungsi utama
int main() {
    Node* root = NULL;
    int choice, priority, duration;
    char name[50];

    do {
        printf("\n--- Property Project Manager ---\n");
        printf("1. Insert Building Type\n");
        printf("2. Show Building Types (Sorted by Duration)\n");
        printf("3. Finish Building Type (Remove by Duration)\n");
        printf("4. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Building Name: ");
                scanf(" %[^\n]", name);
                printf("Enter Priority (1-5): ");
                scanf("%d", &priority);
                printf("Enter Duration (days): ");
                scanf("%d", &duration);
                root = insertBuilding(root, name, priority, duration);
                break;

            case 2:
                printf("\nBuilding List (Sorted by Duration):\n");
                inorder(root);
                break;

            case 3:
                printf("Enter Duration of Finished Building: ");
                scanf("%d", &duration);
                root = finishBuilding(root, duration);
                break;

            case 4:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 4);

    return 0;
}
