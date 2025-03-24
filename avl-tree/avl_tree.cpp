#include <iostream>
#include <utility> // std::pair
#include <algorithm> // std::max
#include <stack>

using namespace std;

// MAX 정의
#define MAX(a,b)    ((a>b) ? (a):(b))

// 올려주신 수도코드를 기반으로 코드를 짰습니다.
// 수도코드에서는 불균형을 한번만 해소하기 때문에, 두번의 rebalacing이 필요한 케이스에 대해 KO가 나옵니다.

typedef int element;

typedef struct _TREENODE_{
    element key;
    struct _TREENODE_* left;
    struct _TREENODE_* right;
    int height;
    int size;
    int bf;
}TREENODE;

bool insertAVL(TREENODE** root, element newKey);
bool deleteAVL(TREENODE** root, element deleteKey);

TREENODE* rotateLL(TREENODE* parent);
TREENODE* rotateLR(TREENODE* parent);
TREENODE* rotateRR(TREENODE* parent);
TREENODE* rotateRL(TREENODE* parent);

bool insertBST(TREENODE** root, element newKey);
TREENODE* deleteBST(TREENODE** root, element deleteKey);

TREENODE* getNodeBST();
TREENODE* getNodeAVL();

void inorderTraverse(TREENODE* root);
void clearTree(TREENODE* node);

// tuple<string, TREENODE*, TREENODE*> checkBalance(TREENODE* root, int newKey);
// void rotateTree(TREENODE** root, string rotateType, TREENODE* p, TREENODE* q);

int height(TREENODE* p);
int size(TREENODE* p);
int bf(TREENODE* p);

int bf(TREENODE* p){
    if(p == NULL){
        return 0;
    }
    return height(p->left)-height(p->right);
}

int height(TREENODE* p){
    int h = 0; // height 구하기
    if(p != NULL){
        h = max(height(p->left),height(p->right))+1;
    }
    return h;
}

int size(TREENODE* p){
    if(p == NULL) return 0;

    return size(p->left)+size(p->right)+1;
}

TREENODE* rotateLL(TREENODE* parent){
    TREENODE* a = parent;
    TREENODE* b = parent->left;
    a->left = b->right;
    b->right = a;

    // a->bf = 0;
    // b->bf = 0;
    a->bf = bf(a);
    b->bf = bf(b);
    return b; // 루트 노드를 반환해서 이어줌 .
}

TREENODE* rotateRR(TREENODE* p){
    TREENODE* a = p;
    TREENODE* b = p->right;
    a->right = b->left;
    b->left = a;

    a->bf = bf(a);
    b->bf = bf(b);
    return b;
}

TREENODE* rotateLR(TREENODE* p){
    TREENODE* a = p;
    TREENODE* b = p->left;
    TREENODE* c = b->right;

    b->right = c->left;
    a->left = c->right;
    c->left = b;
    c->right = a;
    switch(c->bf){
        case 0 : b->bf = 0; a->bf = 0; break;
        case 1 : a->bf = -1; b->bf = 0; break;
        case -1 : b->bf = 1; a->bf = 0; break;
    }
    c->bf = 0;
    b = c;

    return b;
}

TREENODE* rotateRL(TREENODE* p){
    TREENODE* a = p;
    TREENODE* b = p->right;
    TREENODE* c = b->left;

    b->left = c->right;
    a->right = c->left;
    c->left = a;
    c->right = b;
    switch(c->bf){
        case 0 : b->bf = 0; a->bf = 0; break; // c의 bf가 0 이면 .. 0 0 0
        case -1 : a->bf = 1; b->bf = 0; break; // c의 bf가 -1이면 .. a가 1.. b는 0
        case 1 : b->bf = -1; a->bf = 0; break; // c의 bf가 1이면 .. b가 -1 .. a는 0
    }
    c->bf = 0; // c의 bf는 0 .. 
    b = c;

    return b;
}

TREENODE* getNodeBST(){
    TREENODE* newNode = (TREENODE*)calloc(1,sizeof(TREENODE));

    return newNode;
}

TREENODE* getNodeAVL(){
    TREENODE* newNode = (TREENODE*)calloc(1,sizeof(TREENODE));

    return newNode;
}

// rotate함수를 구현할때 매개변수로 "불균형이 발생한 노드"를 주고,
// rotate후 루트노드를 반환하게 하였습니다.
// 그리고 rotate후 루트노드를 "불균형이 발생한 노드"의 부모와 이어주었습니다.

bool insertAVL(TREENODE** root, element newKey){
    TREENODE* p = *root;
    TREENODE* q = NULL;
    TREENODE* x = NULL; // y의 삭제로 인하여 불균형으로 판명된 최초 노드 ..
    TREENODE* f = NULL;

    TREENODE* newNode = NULL;

    stack<TREENODE*> stack;

    // find position to insert newKey while storing parent node on stack
    while(p != NULL){
        if(newKey == p->key){
            return false; // 이미 키가 있으면 false를 반환함.
        }
        q = p;
        stack.push(q);
        
        if(newKey < p->key){
            p = p->left;
        }
        else{
            p = p->right;
        }
    }
    // p == NULL이 되고 q는 삽입한 노드의 position..

    // create new node
    newNode = getNodeAVL();
    newNode->key = newKey;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    newNode->size = 1;
    newNode->bf = 0;

    if(*root == NULL){
        *root = newNode; // T <- y
    }
    else{
        if(newKey < q->key){
            q->left = newNode;
        }
        else{
            q->right = newNode;
        }
    }

    // update height and balancing factor while popping parent node from stack
    while(!stack.empty()){
        q = stack.top(); stack.pop();
        q->size += 1;

        // 1. height함수를 이용해서 q->left와 q->right의 높이를 구한다.
        // q->left와 q->right의 높이를 이용해서 q->bf를 구한다.
        q->bf = bf(q);
        // 큐에서 빼면서 .. 균형인수 초기화해줌.
        if(1 < q->bf || q->bf < -1){
            if(x == NULL){
                x = q; // x -> 불균형으로 판명된 최초의 노드 ..

                if(!stack.empty()){
                    f = stack.top();
                }
            }
        }
    }

    // Step2 : 군형 검사

    // 밸런스를 맞추지 않아도 되는 경우 그냥 끝냄 ..
    if(x == NULL) return true;
    // 불균형 발생한 트리를 rebalacing 하고 부모노드와 이어주는 방식으로 구현하겠습니다.
    
    // f -> 불균형 노드의 부모 !.
    // f가 NULL 인 경우와 f가 있는 경우로 나누어야함.
    x->bf = bf(x);
    if(1 < x->bf){
        x->left->bf = bf(x->left);
        if(0 < x->left->bf){
            if(f == NULL){
                // f가 NULL인 경우 .. 균형을 맞추고 반환되는 노드가 루트노드임
                *root = rotateLL(x);

                // 밸런스를 맞추고 height랑 bf를 다시 다 계산해줘야함.
            }
            else{
                // f가 NULL이 아니면 .. 균형을 맞추고 반환되는 노드를 부모노드랑 이어주면 된다.
                if(f->left == x){
                    f->left = rotateLL(x);
                }
                else{
                    f->right = rotateLL(x);
                }
            }
        }
        else{
            if(f == NULL){
                // f가 NULL인 경우 .. 균형을 맞추고 반환되는 노드가 루트노드임
                *root = rotateLR(x);
            }
            else{
                // f가 NULL이 아니면 .. 균형을 맞추고 반환되는 노드를 부모노드랑 이어주면 된다.
                if(f->left == x){
                    f->left = rotateLR(x);
                }
                else{
                    f->right = rotateLR(x);
                }
            }
        }
    }
    else if(x->bf < -1){
        x->right->bf = bf(x->right);
        if(x->right->bf < 0){
            if(f == NULL){
                // f가 NULL인 경우 .. 균형을 맞추고 반환되는 노드가 루트노드임
                *root = rotateRR(x);
            }
            else{
                // f가 NULL이 아니면 .. 균형을 맞추고 반환되는 노드를 부모노드랑 이어주면 된다.
                if(f->left == x){
                    f->left = rotateRR(x);
                }
                else{
                    f->right = rotateRR(x); 
                }
            }
        }
        else{
            if(f == NULL){
                // f가 NULL인 경우 .. 균형을 맞추고 반환되는 노드가 루트노드임
                *root = rotateRL(x);
            }
            else{
                // f가 NULL이 아니면 .. 균형을 맞추고 반환되는 노드를 부모노드랑 이어주면 된다.
                if(f->left == x){
                    f->left = rotateRL(x);
                }
                else{
                    f->right = rotateRL(x);
                }
            }
        }
    }
    // p-> 불균형 위치, q -> p의 부모

    return true;
}

bool deleteAVL(TREENODE** root, int deleteKey){
    TREENODE* p = *root;
    TREENODE* q = NULL;
    TREENODE* x = NULL;
    TREENODE* f = NULL;

    stack<TREENODE*> stack;
    // 삭제할 키의 위치를 찾는다. p -> 삭제할 키의 위치.
    while(p != NULL && deleteKey != p->key){
        q = p;
        stack.push(q);

        if(deleteKey < p->key){
            p = p->left;
        }
        else{
            p = p->right;
        }
    }

    // 삭제할 키가 트리에 없으면 return
    if(p == NULL){
        return false;
    }

    // 차수가 0인 경우
    if(p->left == NULL && p->right == NULL){
        if(q == NULL){
            *root = NULL; // 부모가 없으면 루트노드 삭제
        }
        else{
            if(q->left == p){
                q->left = NULL;
            }
            else{
                q->right = NULL;
            }
        }

        delete p;
        p = nullptr;
    }
    // 차수가 1인 경우.
    else if(p->left == NULL || p->right == NULL){
        TREENODE* child = NULL;
        
        if(p->left != NULL){
            child = p->left;
        }
        else{
            child = p->right;
        }
        if(q != NULL){
            if(q->left == p){
                q->left = child;
            }
            else{
                q->right = child;
            }
        }
        else{
            // 부모가 없는 경우 p의 자식인 child가 루트노드가 된다.
            *root = child;
        }
        delete p;
        p = nullptr;
    }
    else{
        // 차수가 2인 경우..
        TREENODE* temp = p; // p는 삭제될 노드 .
        stack.push(p);
        p->left->height = height(p->left);
        p->right->height = height(p->right);
        p->left->size = size(p->left);
        p->right->size = size(p->right);

        // rotate하고 height랑 size를 조정하지 않기 때문에 여기서 갱신해줘야함.
        if((p->left->height < p->right->height) || ((p->left->height == p->right->height) && (p->left->size < p->right->size))){
            p = p->right; // p의 오른쪽에서 가장 작은 노드를 찾음.

            // minNode();
            while(p->left != NULL){
                stack.push(p);
                p = p->left;
            }

            q = stack.top(); // q는 후계자 노드의 부모

            // 후계자의 부모와 후계자의 자식을 이어준다.
            if(q->right == p){
                q->right = p->right;
            }
            else{
                q->left = p->right;
            }

            temp->key = p->key;
            temp = p;
            
            delete temp;
            temp = nullptr;
        }
        else{
            // 높이가 왼쪽이 높거나, 높이는 같은데 사이즈가 왼쪽이 커서 삭제하는 경우
            // or 오른쪽 왼쪽 모두 높이와 사이즈가 똑같은 경우
            p = p->left;
            // p의 왼쪽 트리에서 키값이 가장 큰 노드를 찾는다.
            // maxNode();
            while(p->right != NULL){
                stack.push(p);
                p = p->right;
            }

            q = stack.top(); // q는 후계자의 부모 노드를 가리킴.
            // 삭제할 노드의 부모와 삭제할 노드의 자식을 이어준다.
            
            if(q->left == p){
                q->left = p->left;
            }
            else{
                q->right = p->left;
            }

            // 노드 삭제.
            temp->key = p->key;
            temp = p;

            delete temp;
            temp = nullptr;
        }
    }

    while(!stack.empty()){
        q = stack.top(); stack.pop();
        q->height = height(q);
        q->size = size(q);
        
        q->bf = bf(q);

        if(q->bf < -1 || 1 < q->bf){
            x = q;
            f = NULL;

            if(!stack.empty()){
                f = stack.top(); 
            }
            
            // f가 NULL인 경우가 있을 수 있음. (삭제되는 노드가 부모가 없는 경우)
            x->bf = bf(x);
            if(1 < x->bf){
                x->left->bf = bf(x->left);
                if(0 < x->left->bf){
                    if(f == NULL){
                        *root = rotateLL(x);
                    }
                    else{
                        if(f->left == x){
                            f->left = rotateLL(x);
                        }
                        else{
                            f->right = rotateLL(x);
                        }
                    }
                }
                else{
                    if(f == NULL){
                        *root = rotateLR(x);
                    }
                    else{
                        if(f->left == x){
                            f->left = rotateLR(x);
                        }
                        else{
                            f->right = rotateLR(x);
                        }
                    }
                }
            }
            else if(x->bf < -1){
                x->right->bf = bf(x->right);
                if(x->right->bf < 0){
                    if(f == NULL){
                        *root = rotateRR(x);
                    }
                    else{
                        if(f->left == x){
                            f->left = rotateRR(x);
                        }
                        else{
                            f->right = rotateRR(x);
                        }
                    }
                }
                else{
                    if(f == NULL){
                        *root = rotateRL(x);
                    }
                    else{
                        if(f->left == x){
                            f->left = rotateRL(x);
                        }
                        else{
                            f->right = rotateRL(x);
                        }
                    }
                }
            }
        }
    }

    return true;
}

// 후위순회로 clear
void clear(TREENODE* __x){
    if(__x == nullptr) return;

    clear(__x->left);
    clear(__x->right);

    delete __x;
    __x = nullptr;
}

void inorder(TREENODE* node){
    if(node == NULL){
        return;
    }
    cout << "<";
    inorder(node->left);
    cout << " " << node->key << " ";
    inorder(node->right);
    cout << ">";
}

int main(){
    char command;
    int key;

    TREENODE* root = NULL;

    while(cin >> command >> key){
        if(command == 'i'){
            if(insertAVL(&root,int(key)) == false){
                printf("i %d: The key already exists\n", key);
                continue;
            }
        }
        else if(command == 'd'){
            if(deleteAVL(&root, int(key)) == false){
                printf("d %d: The key does not exist\n", key);
                continue;
            }
        }
        else{
            cout << "Invalid command: " << command << "\n";
            return (1);
        }

        inorder(root);
        cout << "\n";
    }

    clear(root);

    return (0);
}
