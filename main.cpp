#include "functions.h"
#include "assert.h"

int main (int argc, char** argv)
{
    printf("\n");

    if (argc < 2)
    {
        printf(BRIGHT RED "Please specify the data file like this:" NRM " %s data.txt\n", argv[0]);
        return 0;
    }

    data_base my_data = {};
    if (ReadDataFromFile(&my_data, argv[1]) != 0)
    {
        printf(BRIGHT RED "Cannot open file %s\n" NRM, argv[1]);
        return 0;
    }

    printf(MAGENTA "- Here's what I can do:\n" NRM "1) Play akinator\n2) Say definition of something\n3) Say all things I know\n4) Quit\n");

    while (1)
    {
        int choise = 0;
        char* name;
        printf(MAGENTA "- Enter number (1-4): " NRM);
        if (scanf("%i", &choise) != 1)
        {
            ClearInput();
            continue;
        }
        switch (choise)
        {
            case 1:
                printf(MAGENTA "\n- Let's start! Pick a thing and I will try to guess it\n- Are you ready?\n" NRM);
                while (!(BiQuestion()))
                {
                    printf(MAGENTA "- It's okay, you have plenty of time. Enter <yes> when you're ready\n" NRM);
                }
                Akinator(&my_data);
                break;
            case 2:
                printf(MAGENTA "\nEnter name of the thing you want to define: " NRM);

                name = ScanStringColor(INTENSE_GREEN);
                PrintOrigin(&my_data, name);
                break;
            case 3:
                PrintAnswersAlphabetically(&my_data);
                printf("\n");
                break;
            case 4:
                FreeData(&my_data);
                return 0;
            default:
                ClearInput();
                continue;
        }

        printf(MAGENTA "\n- Start again?\n" NRM);
        if (BiQuestion())
        {
            printf(MAGENTA "- Here's what I can do:\n" NRM "1) Play akinator\n2) Say definition of something\n3) Say all things I know\n4) Quit\n");
            continue;
        }
        else
        {
            FreeData(&my_data);
            return 0;
        }
    }

    // PrintOrigin(&my_data, "Dave");
    // PrintAnswersAlphabetically(&my_data);
    // Akinator(&my_data);
    // SaveDataToFile(&my_data, argv[1]);

}
