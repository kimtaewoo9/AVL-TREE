# AVL-TREE

AVL Tree Implementation Project
This project is a C++ implementation of an AVL tree (self-balancing binary search tree). AVL trees maintain balance after insertion and deletion operations, ensuring O(log n) time complexity for all operations.

Features
Insert new keys into the AVL tree (`insertAVL`)

Delete keys from the AVL tree (`deleteAVL`)

Rotation operations to maintain tree balance:

LL rotation (`rotateLL`)

RR rotation (`rotateRR`)

LR rotation (`rotateLR`)

RL rotation (`rotateRL`)

Inorder traversal to display the tree (`inorder`)

Implementation Details
Each node stores a key value, left child pointer, right child pointer, height, size, and balance factor (BF)

The balance factor (BF) is calculated as the height of the left subtree minus the height of the right subtree

After insertion or deletion, if the balance factor falls outside the range of -1, 0, 1, appropriate rotation operations are performed to restore tree balance

Usage

The program accepts the following commands:

```
i key: Insert a key value into the AVL tree
d key: Delete a key value from the AVL tree
After each command, the current state of the tree is displayed using inorder traversal.
```

Example

```
i 10
< <> 10 <> >
i 20
< <> 10 < <> 20 <> > >
i 30
< <> 20 < <> 10 <> > < <> 30 <> > >
d 10
< <> 20 < <> 30 <> > >
```

Note

The code is based on pseudocode and resolves imbalances only once. Therefore, it may not function correctly in cases where two rebalancing operations are required.
