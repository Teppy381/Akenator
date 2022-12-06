#include "functions.h"
#include "assert.h"

int main ()
{
    printf("\n");

    data_base my_data = {};
    ReadDataFromFile(&my_data, "data.txt");


    // PrintOrigin(&my_data, "Spiner");

    // PrintAnswersAlphabetically(&my_data);

    // Akenator(&my_data);


    FILE* save_file = fopen("data.txt", "w");
    SaveDataToFile(&my_data, save_file);

    FreeData(&my_data);
}
