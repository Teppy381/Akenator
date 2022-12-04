#include "functions.h"


int CreateTreeFromFile(tree_node* node_p, const char* file_name)
{
    FILE* data_file = fopen(file_name, "r");
    char* buffer = ReadFile(data_file);
    fclose(data_file);

    int counter = 0;

    GetAll(buffer, node_p, &counter);

    free(buffer);
    return 0;
}

void GetAll(char* str, tree_node* node_p, int* counter)
{
    GetSomething(str, node_p, counter);

    CHECKUS(str[*counter] == '\0', (void) 0);
}

void GetSomething(char* str, tree_node* node_p, int* counter)
{
    if (str[*counter] == '{')
    {
        GetQuession(str, node_p, counter);
    }
    else
    {
        GetAnswer(str, node_p, counter);
    }
}

void GetQuession(char* str, tree_node* node_p, int* counter)
{
    CHECKUS(str[*counter] == '{', (void) 0);
    (*counter)++;
    CHECKUS(str[*counter] == '\n', (void) 0);
    (*counter)++;

    AddLeftBranch(node_p, NULL);
    AddRightBranch(node_p, NULL);

    sscanf(&str[*counter], "%m[^?\n]", &node_p->data);
    *counter += strlen(node_p->data);

    CHECKUS(str[*counter] == '?', (void) 0);
    (*counter)++;
    CHECKUS(str[*counter] == '\n', (void) 0);
    (*counter)++;

    GetSomething(str, node_p->left, counter);

    CHECKUS(str[*counter] == '\n', (void) 0);
    (*counter)++;

    GetSomething(str, node_p->right, counter);

    CHECKUS(str[*counter] == '\n', (void) 0);
    (*counter)++;
    CHECKUS(str[*counter] == '}', (void) 0);
    (*counter)++;
}

void GetAnswer(char* str, tree_node* node_p, int* counter)
{
    sscanf(&str[*counter], "%m[^\n]", &node_p->data);
    *counter += strlen(node_p->data);
}




char* ReadFile(FILE* text_input)
{
    CHECKUS(text_input != NULL, NULL);

    int simbol_amount = CountSimbols(text_input);

    char* buffer = (char*) calloc(simbol_amount + 1, sizeof(*buffer));
    CHECKUS(buffer != NULL, NULL);

    fread(buffer, sizeof(*buffer), simbol_amount, text_input);
    buffer[simbol_amount - 1] = '\0';

    return buffer;
}


int CountSimbols(FILE* text_input)
{
    CHECKUS(text_input != NULL, 1);

    fseek(text_input, 0, SEEK_END);
    int simbol_amount = ftell(text_input);
    fseek(text_input, 0, SEEK_SET);

    return simbol_amount;
}

int Akenator(tree_node* node_p)
{

    while (1)
    {
        if (node_p->left != 0 && node_p->right != 0) // green?
        {
            printf("- %s?\n", node_p->data);
            if (BiQuestion() == 1)
            {
                node_p = node_p->right;
            }
            else
            {
                node_p = node_p->left;
            }
        }
        else if (node_p->left == 0 && node_p->right == 0) // Is it apple?
        {
            printf("- Is it " INTENSE_RED "%s" CLR_COLOR "?\n", node_p->data);
            if (BiQuestion() == 1)
            {
                printf("I knew that\n");
                return 0;
            }
            else
            {
                printf("- Now I'm sad (((\n");
                AddQuestionNode(node_p);
                return 0;
            }
        }
        else // error
        {
            printf("Error on node with data: %s\n", node_p->data);
            return 1;
        }
    }
}

int AddQuestionNode(tree_node* node_p)
{
    printf("- Can you say me what it was?\n");
    if (BiQuestion() == 0)
    {
        printf("- Now I'm sad (((\n");
        return 0;
    }


    printf("- Complete the sentence: It was ");
    char* buf1 = ScanStringColor(INTENSE_GREEN);


    AddRightBranch(node_p, buf1);
    AddLeftBranch(node_p, node_p->data);

    printf("- Complete the sentence: Differience between " INTENSE_RED "%s" CLR_COLOR " and " INTENSE_GREEN "%s"
    CLR_COLOR " is that " INTENSE_GREEN "%s" CLR_COLOR " ", node_p->data, buf1, buf1);
    char* buf2 = ScanStringColor(INTENSE_BLUE);

    node_p->data = buf2;

    printf("- Thanks, now I know everything!!!\n");
    return 0;
}

int BiQuestion()
{
    char* buf = (char*) calloc(40, sizeof(char));

    scanf("%s", buf);
    if (strcmp(buf, "y") == 0 || strcmp(buf, "yes") == 0 || strcmp(buf, "Y") == 0 || strcmp(buf, "YES") == 0)
    {
        free(buf);
        return 1;
    }
    else if (strcmp(buf, "n") == 0 || strcmp(buf, "no") == 0 || strcmp(buf, "N") == 0 || strcmp(buf, "NO") == 0)
    {
        free(buf);
        return 0;
    }

    while (1)
    {
        ClearInput();
        printf("Enter y (yes) or n (no): ");
        scanf("%s", buf);
        if (strcmp(buf, "y") == 0 || strcmp(buf, "yes") == 0 || strcmp(buf, "Y") == 0 || strcmp(buf, "YES") == 0)
        {
            free(buf);
            return 1;
        }
        else if (strcmp(buf, "n") == 0 || strcmp(buf, "no") == 0 || strcmp(buf, "N") == 0 || strcmp(buf, "NO") == 0)
        {
            free(buf);
            return 0;
        }
    }
}

void ClearInput()
{
    while (getchar() != '\n');
}

char* ScanStringColor(const char* color)
{
    ClearInput();

    printf("%s", color);
    char* str = NULL;
    if (scanf(" %m[^\n\r{}]", &str) != 1) // includes calloc
    {
        printf(BRIGHT RED "Cannot read string\n" NRM);
        exit(1);
    }
    printf("%s", NRM);

    int first_letter = 0;
    while (isspace(str[first_letter]))
    {
        first_letter++;
    }
    if (islower(str[first_letter]))
    {
        str[first_letter] -= 32;
    }
    int last_letter = 1;
    for (int i = first_letter; str[i] != '\0'; i++)
    {
        if (!(isspace(str[i])))
        {
            last_letter = i;
        }
    }
    str[last_letter + 1] = '\0';
    return &str[first_letter]; // may be dangerous to return different pointer from that was calloced
}

int SaveTreeToFile(const tree_node* node_p, FILE* file)
{
    if (node_p == 0)
    {
        return 0;
    }

    if (node_p->left != 0 && node_p->right != 0)
    {
        fprintf(file, "{\n%s?\n", node_p->data);
        SaveTreeToFile(node_p->left, file);
        SaveTreeToFile(node_p->right, file);
        fprintf(file, "}\n");
    }
    else if (node_p->left == 0 && node_p->right == 0)
    {
        fprintf(file, "%s\n", node_p->data);
    }
    return 0;
}

int FreeTree(tree_node* node_p)
{
    if (node_p == 0)
    {
        return 0;
    }
    if (node_p->left != 0)
    {
        FreeTree(node_p->left);
    }
    if (node_p->right != 0)
    {
        FreeTree(node_p->right);
    }

    free(node_p->data);

    if (node_p->parent != 0)
    {
        free(node_p);
    }
    return 0;
}

int AddRightBranch(tree_node* node_p, char* data)
{
    CHECKUS(node_p != 0, 1);

    node_p->right = (tree_node*) calloc(1, sizeof(tree_node));
    node_p->right->parent = node_p;
    node_p->right->data = data;

    return 0;
}

int AddLeftBranch(tree_node* node_p, char* data)
{
    CHECKUS(node_p != 0, 1);

    node_p->left = (tree_node*) calloc(1, sizeof(tree_node));
    node_p->left->parent = node_p;
    node_p->left->data = data;

    return 0;
}

