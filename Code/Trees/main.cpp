#include <iostream>
#include <random>
#include "bstree.h"
#include "redBlackTree.h"

using namespace std;

#define OPF(b) ((b) ? "pass" : "fail")

const uint32_t
    DEFAULT_N_TREES = 4,
    DEFAULT_N_KEYS = 128,
    REGULAR_THRESHOLD = 100000;

RedBlackTree<uint64_t,uint32_t>
    *ordered;
SortedLinearList<uint64_t>
    *bst;

uint32_t
    nKeys = DEFAULT_N_KEYS;

void orderedInserter(const uint64_t &k,uint32_t &v) {

    (*ordered)[k] = v;
    if (nKeys <= REGULAR_THRESHOLD)
        bst->insert(k);
}

int main(int argc,char *argv[]) {
    RedBlackTree<uint64_t,uint32_t>
        *trees;
    uint64_t
        **keys;
    uint32_t
        **values,
        nTrees = DEFAULT_N_TREES;
    random_device
        rd;
    mt19937
        mt(rd());
    uniform_int_distribution<>
        dis(0,0x3fffffff);
    bool
        okay;

    bst = new SortedLinearList<uint64_t>;

    // get counts
    REPI(i,1,argc)
        if (argv[i][0] == '-') {
            if (argv[i][1] == 't')
                nTrees = strtol(argv[i]+2, nullptr,10);
            if (argv[i][1] == 'k')
                nKeys = strtol(argv[i]+2, nullptr,10);
        }

    // allocate spaces
    trees = new RedBlackTree<uint64_t,uint32_t>[nTrees];
    keys = new uint64_t *[nTrees];
    values = new uint32_t *[nTrees];

    // fill spaces

    REPI(i,0,nTrees) {
        keys[i] = new uint64_t[nKeys];
        values[i] = new uint32_t[nKeys];

        REPI(j,0,nKeys) {
            keys[i][j] = (((uint64_t)dis(mt)) << 32) | j;
            values[i][j] = dis(mt);
        }
    }

    // test empty, size and height

    cout << "Empty tree tests:" << endl;
    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && trees[i].isEmpty();
    }
    cout << "isEmpty(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && (trees[i].size() == 0);
    }
    cout << "   size(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && (trees[i].height() == 0);
    }
    cout << " height(): " << OPF(okay) << endl;

    // test [] to insert half of the values

    cout << "\nInserting half of values into tree:" << endl;
    REPI(i,0,nTrees)
        REPI(j,0,nKeys/2)
            trees[i][keys[i][2*j]] = values[i][2*j];

    // verify tree properties

    okay = true;
    REPI(i,0,nTrees) {
        try {
            trees[i].isValidRBTree();
        } catch (const logic_error &e) {
            cout << "Tree " << i << ": " << e.what() << endl;
            okay = false;
        }
    }
    cout << "insert half: " << OPF(okay) << endl;

    // test empty, size and height

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && !trees[i].isEmpty();
    }
    cout << "  isEmpty(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && (trees[i].size() == nKeys / 2);
    }
    cout << "     size(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && (trees[i].height() != 0);
    }
    cout << "   height(): " << OPF(okay) << endl;

    // test search

    okay = true;
    REPI(i,0,nTrees)
        REPI(j,0,nKeys)
            try {
                if (trees[i].search(keys[i][j]) != values[i][j])
                    okay = false;
                if (j % 2 == 1)
                    okay = false;
            } catch (const domain_error &e) {
                if (j % 2 == 0)
                    okay = false;
            }
    cout << "   search(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees)
        REPI(j,0,nKeys/2)
            try {
                if (trees[i][keys[i][2*j]] != values[i][2*j]) {
                    okay = false;
                    cout << "tree " << i << " key " << keys[i][j] << " values " << values[i][j]
                        << " tree reports " << trees[i][keys[i][j]] << endl;
                }
            } catch (const domain_error &e) {
            }
    cout << " operator[]: " << OPF(okay) << endl;

    // insert remaining values

    cout << "\nInserting remaining values into tree:" << endl;
    REPI(i,0,nTrees)
        REPI(j,0,nKeys/2)
            trees[i][keys[i][2*j+1]] = values[i][2*j+1];

    // test search, size, height

    okay = true;
    REPI(i,0,nTrees)
        REPI(j,0,nKeys)
            try {
                if (trees[i].search(keys[i][j]) != values[i][j])
                    okay = false;
            } catch (const domain_error &e) {
                    okay = false;
            }
    cout << "   search(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees)
        REPI(j,0,nKeys)
            try {
                if (trees[i][keys[i][j]] != values[i][j]) {
                    okay = false;
                    cout << "tree " << i << " key " << keys[i][j] << " values " << values[i][j]
                         << " tree reports " << trees[i][keys[i][j]] << endl;
                }
            } catch (const domain_error &e) {
            }
    cout << " operator[]: " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && !trees[i].isEmpty();
    }
    cout << "  isEmpty(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && (trees[i].size() == nKeys);
    }
    cout << "     size(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && (trees[i].height() != 0);
    }
    cout << "   height(): " << OPF(okay) << endl;

    // remove half of the values

    cout << "\nRemoving half of keys:" << endl;
    REPI(i,0,nTrees)
        REPI(j,0,nKeys/2)
            trees[i].remove(keys[i][2*j]);

    // verify tree properties

    okay = true;
    REPI(i,0,nTrees) {
        try {
            trees[i].isValidRBTree();
        } catch (const logic_error &e) {
            cout << "Tree " << i << ": " << e.what() << endl;
            okay = false;
        }
    }
    cout << "valid trees: " << OPF(okay) << endl;

    // verify via search

    okay = true;
    REPI(i,0,nTrees)
        REPI(j,0,nKeys)
            try {
                if (trees[i].search(keys[i][j]) != values[i][j])
                    okay = false;
                if (j % 2 == 0)
                    okay = false;
            } catch (const domain_error &e) {
                if (j % 2 == 1)
                    okay = false;
            }
    cout << "   search(): " << OPF(okay) << endl;

    // test [] for update

    REPI(i,0,nTrees)
        REPI(j,0,nKeys/2)
            trees[i][keys[i][2*j+1]] = trees[i][keys[i][2*j+1]] * 2;

    okay = true;
    REPI(i,0,nTrees)
        REPI(j,0,nKeys/2)
            try {
                if (trees[i].search(keys[i][2*j+1]) != 2*values[i][2*j+1])
                    okay = false;
            } catch (const domain_error &e) {
                if (j % 2 == 1)
                    okay = false;
            }
    cout << " operator[]: " << OPF(okay) << endl;

    // test clear on one tree

    cout << "\nClearing tree 0:" << endl;
    trees[0].clear();

    // test empty

    cout << "isEmpty(): " << OPF(trees[0].isEmpty()) << endl;

    cout << "   size(): " << OPF(trees[0].size() == 0) << endl;
    cout << " height(): " << OPF(trees[0].height() == 0) << endl;

    // take another tree and copy into cleared tree in order

    cout << "\nOrdered insert:" << endl;
    ordered = trees;
    trees[1].map(orderedInserter);

    // verify tree properties

    okay = true;
    try {
        trees[0].isValidRBTree();
    } catch (const logic_error &e) {
        cout << e.what() << endl;
        okay = false;
    }
    cout << "valid: " << OPF(okay) << endl;

    // check height

    cout << "  ordered insert size: " << trees[0].size() << endl;
    cout << "ordered insert height: " << trees[0].height() << endl;
    cout << "   regular BST height: " << bst->height() << endl;

    // remove remaining nodes

    cout << "\nRemoving all keys:" << endl;
    okay = true;
    REPI(i,0,nKeys/2)
        try {
            trees[0].remove(keys[1][2 * i + 1]);
        } catch (const domain_error &e) {
            okay = false;
        }

    REPI(i,1,nTrees)
        REPI(j,0,nKeys/2)
            try {
                trees[i].remove(keys[i][2*j+1]);
            } catch (const domain_error &e) {
                okay = false;
            }
    cout << " remove(): " << OPF(okay) << endl;

    // verify empty, size, height

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && trees[i].isEmpty();
    }
    cout << "isEmpty(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && (trees[i].size() == 0);
    }
    cout << "   size(): " << OPF(okay) << endl;

    okay = true;
    REPI(i,0,nTrees) {
        okay = okay && (trees[i].height() == 0);
    }
    cout << " height(): " << OPF(okay) << endl;

    return 0;
}
