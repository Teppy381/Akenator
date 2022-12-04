#include "functions.h"
#include "assert.h"

int main ()
{
    printf("\n");

    tree_node tree = {};
    CreateTreeFromFile(&tree, "data.txt");

    Akenator(&tree);

    FILE* my_data = fopen("data.txt", "w");
    SaveTreeToFile(&tree, my_data);

    FreeTree(&tree);
}
