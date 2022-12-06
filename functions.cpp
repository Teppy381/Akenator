#include "functions.h"


int ReadDataFromFile(data_base* data_p, const char* file_name)
{
    FILE* data_file = fopen(file_name, "r");
    char* buffer = ReadFile(data_file);
    fclose(data_file);

    data_p->answers_list = (tree_node**) calloc(100, sizeof(tree_node*));
    data_p->tree = (tree_node*) calloc(1, sizeof(tree_node));

    int counter = 0;
    GetAll(buffer, data_p->tree, &counter, data_p->answers_list);

    free(buffer);
    return 0;
}

void GetAll(char* str, tree_node* node_p, int* counter, tree_node** answers_list)
{
    CHECKUS(node_p != NULL, (void) 0);
    GetSomething(str, node_p, counter, answers_list);

    CHECKUS(str[*counter] == '\0', (void) 0);
}

void GetSomething(char* str, tree_node* node_p, int* counter, tree_node** answers_list)
{
    CHECKUS(node_p != NULL, (void) 0);
    if (str[*counter] == '{')
    {
        GetQuession(str, node_p, counter, answers_list);
    }
    else
    {
        GetAnswer(str, node_p, counter, answers_list);
    }
}

void GetQuession(char* str, tree_node* node_p, int* counter, tree_node** answers_list)
{
    CHECKUS(node_p != NULL, (void) 0);

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

    GetSomething(str, node_p->left, counter, answers_list);

    CHECKUS(str[*counter] == '\n', (void) 0);
    (*counter)++;

    GetSomething(str, node_p->right, counter, answers_list);

    CHECKUS(str[*counter] == '\n', (void) 0);
    (*counter)++;
    CHECKUS(str[*counter] == '}', (void) 0);
    (*counter)++;
}

void GetAnswer(char* str, tree_node* node_p, int* counter, tree_node** answers_list)
{
    CHECKUS(node_p != NULL, (void) 0);
    sscanf(&str[*counter], "%m[^\n]", &node_p->data);
    *counter += strlen(node_p->data);

    AddNodeToList(answers_list, node_p);
}


void AddNodeToList(tree_node** answers_list, tree_node* node_p)
{
    CHECKUS(node_p != NULL, (void) 0);
    CHECKUS(answers_list != 0, (void) 0);
    int i = 0;
    while (answers_list[i] != 0)
    {
        i++;
    }
    answers_list[i] = node_p;
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

int Akenator(data_base* data_p)
{
    tree_node* node_p = data_p->tree;
    while (1)
    {
        if (node_p->left != 0 && node_p->right != 0) // green?
        {
            printf(MAGENTA "- %s?\n" CLR_COLOR, node_p->data);
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
            printf(MAGENTA "- Is it " INTENSE_RED "%s" MAGENTA "?\n" CLR_COLOR, node_p->data);
            if (BiQuestion() == 1)
            {
                printf(MAGENTA "- I knew that\n" CLR_COLOR);
                return 0;
            }
            else
            {
                printf(MAGENTA "- Now I'm sad (((\n" CLR_COLOR);
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
    printf(MAGENTA "- Can you say me what it was?\n" CLR_COLOR);
    if (BiQuestion() == 0)
    {
        printf(MAGENTA "- Now I'm sad (((\n\n" CLR_COLOR);
        return 0;
    }


    printf(MAGENTA "- Complete the sentence: " CLR_COLOR "It was ");
    char* buf1 = ScanStringColor(INTENSE_GREEN);


    AddRightBranch(node_p, buf1);
    AddLeftBranch(node_p, node_p->data);

    printf(MAGENTA "- Complete the sentence: " CLR_COLOR "Differience between " INTENSE_RED "%s" CLR_COLOR " and " INTENSE_GREEN "%s"
    CLR_COLOR " is that " INTENSE_GREEN "%s" CLR_COLOR " ", node_p->data, buf1, buf1);
    char* buf2 = ScanStringColor(INTENSE_BLUE);

    node_p->data = buf2;

    printf(MAGENTA "- Thanks, now I know everything!!!\n\n" CLR_COLOR);
    return 0;
}


int PrintOrigin(data_base* data_p, const char* str)
{
    CHECKUS(data_p != NULL, 1);

    tree_node* buf_node_p = 0;
    for (int i = 0; data_p->answers_list[i] != 0; i++)
    {
        if (strcmp(str, data_p->answers_list[i]->data) == 0)
        {
            buf_node_p = data_p->answers_list[i];
            break;
        }
    }
    if (buf_node_p == 0) // did not find
    {
        printf("Cannot find <%s> in data base\n", str);
        return 1;
    }

    printf(MAGENTA "- Defenition of %s:\n" CLR_COLOR, str);
    PrintParents(buf_node_p->parent);
    printf("\n");
    return 0;
}

int PrintParents(tree_node* node_p)
{
    CHECKUS(node_p != NULL, 1);

    if (node_p->parent != 0)
    {
        PrintParents(node_p->parent);
    }
    printf("%s\n", node_p->data);
    return 0;
}

int PrintAnswersAlphabetically(data_base* data_p)
{
    CHECKUS(data_p != NULL, 1);

    tree_node** answers_list = data_p->answers_list;

    SortAnswersAlphabetically(answers_list);

    printf(MAGENTA "- Here is all things I know:\n" CLR_COLOR);
    for (int i = 0; answers_list[i] != 0; i++)
    {
        printf("%i) %s\n", i + 1, answers_list[i]->data);
    }
    printf("\n");
    return 0;
}

int SortAnswersAlphabetically(tree_node** answers_list)
{
    int answers_amount = 0;
    while (answers_list[answers_amount] != 0)
    {
        answers_amount++;
    }

    qsort(answers_list, answers_amount, sizeof(tree_node*), AlphabetAnswersComparator);
    return 0;
}

int AlphabetAnswersComparator(const void* answer1, const void* answer2)
{
    const char* str1 = (*(tree_node**)answer1)->data;
    const char* str2 = (*(tree_node**)answer2)->data;

    return strcmp(str1, str2);
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

int SaveDataToFile(data_base* data_p, FILE* file)
{
    SaveTreeToFile(data_p->tree, file);
    return 0;
}

int SaveTreeToFile(const tree_node* node_p, FILE* file)
{
    CHECKUS(node_p != NULL, 1);

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


int FreeData(data_base* data_p)
{
    FreeTree(data_p->tree);
    free(data_p->answers_list);
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

