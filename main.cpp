#include "D:\1_UNIVERSITY\HK201\4_DataStructure_n_Algorithms\ASM\ASM_2\BKUTree.h"

void printKey(int key, int value) {
    cout << key << " ";
}

void testAVLTree() 
{
    try 
    {
        BKUTree<int, int>::AVLTree* tree = new BKUTree<int, int>::AVLTree();
        int keys[] = { 9, 25, 30, 48, 1, 3, 8, 22, 33 }; // , 11, 13, 37, 2, 36, 43, 26, 46, 23, 24, 39, 7, 18, 42, 49, 5, 32, 10, 20, 16, 28, 38, 40 };
        int size = sizeof(keys) / sizeof(keys[0]);
        for (int i = 0; i < size; i++) {
            tree->add(keys[i], keys[i]);
        }
        cout << "INITIAL STATE:\n";
        cout << "AVL tree: \n";
        tree->printTreeStructure();
        cout << "\n==============================\n";
        // tree->remove(33);

        int input = 0;
        while (input != -1) {
            cout << "Remove key: ";
            // cout << "Search: ";
            cin >> input;
            
            tree->remove(input);
            // tree->clear();
            cout << "\nAVL tree: \n";
            tree->printTreeStructure();
            // tree->traverseNLR(printKey);
            // cout << '\n';
            cout << "\n\n";
        }
    }
    catch (const char* er) {
        cerr << er << endl;
        testAVLTree();
    }
}

void testSplayTree() 
{
    try 
    {
        BKUTree<int, int>::SplayTree* tree = new BKUTree<int, int>::SplayTree();
        int keys[] = { 1,3,5,9,7,2,0,4,6,8,-1,20,10 }; // , 11, 13, 37, 2, 36, 43, 26, 46, 23, 24, 39, 7, 18, 42, 49, 5, 32, 10, 20, 16, 28, 38, 40 };
        int size = sizeof(keys) / sizeof(keys[0]);
        for (int i = 0; i < size; i++) {
            tree->add(keys[i], keys[i]);
        }
        cout << "INITIAL STATE:\n";
        cout << "AVL tree: \n";
        tree->printTreeStructure();
        cout << "\n==============================\n";
        // tree->remove(33);

        int input = 0;
        while (input != -1) {
            cout << "Remove key: ";
            // cout << "Search: ";
            cin >> input;
            
            tree->remove(input);
            // tree->clear();
            cout << "\nAVL tree: \n";
            tree->printTreeStructure();
            // tree->traverseNLR(printKey);
            // cout << '\n';
            cout << "\n\n";
        }
    }
    catch (const char* er) {
        cerr << er << endl;
        testSplayTree();
    }
}

void testBKUTree()
{
    try 
    {
        BKUTree<int, int>* tree = new BKUTree<int, int>();
        vector<int> traverseList;
        int keys[] = { 9, 25, 30, 48, 1, 3, 8, 22, 33 }; // , 11, 13, 37, 2, 36, 43, 26, 46, 23, 24, 39, 7, 18, 42, 49, 5, 32, 10, 20, 16, 28, 38, 40 };
        int size = sizeof(keys) / sizeof(keys[0]);
        for (int i = 0; i < size; i++) {
            tree->add(keys[i], keys[i]);
        }

        cout << "INITIAL STATE:\n";
        cout << "AVL tree: \n";
        tree->avl->printTreeStructure();
        cout << "Splay tree: \n";
        tree->splay->printTreeStructure();
        cout << "Current queue: \n";
        queue<int> shift;
        while (!tree->keys.empty()) {
            cout << tree->keys.front() << " ";
            shift.push(tree->keys.front());
            tree->keys.pop();
        }
        while (!shift.empty()) {
            tree->keys.push(shift.front());
            shift.pop();
        }
        cout << "\n==============================\n";
        // tree->search(9, traverseList);
        // cout << "AVL:\n";
        // tree->traverseNLROnAVL(printKey);
        // cout << "Splay:\n";
        // tree->traverseNLROnSplay(printKey);


        // tree->printTreeStructure();
        int input = 0;
        while (input != -1) {
            cout << "Remove key: ";
            // cout << "Search: ";
            cin >> input;
            
            tree->remove(input);

            // tree->clear();
            
            // cout << "Value at key is: " << tree->search(input, traverseList) << '\n';
            // cout << "Traverse list: ";
            // for (int i : traverseList) cout << i << " ";
            cout << '\n';
            
            cout << "AVL tree: \n";
            tree->avl->printTreeStructure();
            // tree->traverseNLROnAVL(printKey);
            // cout << '\n';
            
            cout << "Splay tree: \n";
            tree->splay->printTreeStructure();
            // tree->traverseNLROnSplay(printKey);

            cout << "Current queue: \n";
            while (!tree->keys.empty()) {
                cout << tree->keys.front() << " ";
                shift.push(tree->keys.front());
                tree->keys.pop();
            }
            while (!shift.empty()) {
                tree->keys.push(shift.front());
                shift.pop();
            }
            cout << "\n\n";
            while (!traverseList.empty()) traverseList.pop_back();
        }
        // tree->search(13, traverseList);
        
        // tree->printTreeStructure();
        // cout << "----------------------------\n";
        // tree->remove(11);
        // tree->printTreeStructure();
        // cout << "----------------------------\n";
        // tree->remove(48);
        // tree->printTreeStructure();

    }
    catch (const char* er) {
        cerr << er << endl;
        testBKUTree();
    }
}

int main() 
{
    // testBKUTree();
    // int test;
    // cin >> test;
    // switch (test)
    // {
    // case 1:
    //     testAVLTree();
    //     break;
    // case 2:
    //     testSplayTree();
    //     break;
    // default:
    //     testBKUTree();
    // }

    BKUTree<int, int>* BKU = new BKUTree<int, int>();
	int arr[] = {1,3,5,9,7,2,0,4,6,8,-1,20,10};
	for (int i = 0; i < 13; i++){
		BKU->add(arr[i],i); 
        cout << "SPLAY: \n";
        // BKU->splay->printTreeStructure();
        BKU->traverseNLROnSplay(printKey); 
        cout << endl << "AVL  :\n";
        // BKU->avl->printTreeStructure();
        BKU->traverseNLROnAVL(printKey); 
        cout << endl << "Queue: "; 
        BKU->InQueue(); 
        cout << endl << endl;		
	}
	cout << "==============================================================================\n";
	vector<int> A;
	cout << BKU->search(5,A) << endl; 
	while(!A.empty())
    {
		cout << A.back() <<" ";
		A.pop_back();
	} 
    cout << endl;
	cout << "SPLAY: ";
    BKU->traverseNLROnSplay(printKey); 
    cout << endl << "AVL  : ";
    BKU->traverseNLROnAVL(printKey); 
    cout << endl << "Queue: "; 
    BKU->InQueue(); 
    cout << endl << endl;
	//BKU->traverseNLROnAVL(INT);	
	//auto p = BKU->splay->root->corr;				cout<<typeid(p).name() <<endl;
	cout << "==============================================================================\n";
	for (int i = 12; i > -1; i--){
        cout << "-- Remove key " << arr[i] << '\n';
		BKU->remove(arr[i]);
        cout << "SPLAY: ";
        BKU->traverseNLROnSplay(printKey); 
        cout << endl << "AVL  : ";
        BKU->traverseNLROnAVL(printKey); 
        cout << endl << "Queue: "; 
        BKU->InQueue(); 
        cout << endl << endl;
	}

    return 0;
}