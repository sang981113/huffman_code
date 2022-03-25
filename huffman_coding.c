#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#define ALPHABET 26 
#define MAX_LEN 255 
#define MAX_ELEMENT 1000 

typedef struct {
    int alpha; // ���ĺ�
    int freq; // �󵵼�
}AlphaType;

typedef struct TreeNode {
    AlphaType weight;
    struct TreeNode* left_child;
    struct TreeNode* right_child;
}TreeNode;

typedef struct {
    TreeNode* pTree;
    int key;
}Element;

typedef struct {
    Element heap[MAX_ELEMENT];
    int heap_size;
}HeapType;

typedef struct {
    char alpha;
    char* code;
}HuffmanTable;

void Init(AlphaType* p)
{
    for (int i = 0; i < ALPHABET; i++)
    {
        p[i].alpha = i + 65;
        p[i].freq = 0;
    }
}

void initHuffmanTable(HuffmanTable* ht) {
    ht->alpha = '\0';
    ht->code = (char*)malloc(sizeof(char));
}

void InsertHeap(HeapType* h, Element item)
{
    int i;
    i = ++(h->heap_size);

    while (i != 1 && item.key < h->heap[i / 2].key) // �θ���� �� 
    {
        h->heap[i] = h->heap[i / 2];
        i /= 2;
    }

    h->heap[i] = item;
}

Element DeleteHeap(HeapType* h)
{
    int parent = 1, child = 2;
    Element data, temp;

    data = h->heap[parent];
    temp = h->heap[(h->heap_size)--]; // ������ ���� size ���� 

    while (child <= h->heap_size)
    {
        //�ڽ� ��尣 ���� �� ��  
        if ((child < h->heap_size) && (h->heap[child].key) > h->heap[child + 1].key)
            child++;

        if (temp.key <= h->heap[child].key) break;

        h->heap[parent] = h->heap[child];
        parent = child;
        child *= 2; // ���� �ڽ� ���� �� 
    }

    h->heap[parent] = temp;
    return data;
}

TreeNode* MakeNode(TreeNode* left, TreeNode* right)
{//���� �Ҵ����� ����, DestroyTree()�� ���� �� ���� ȸ��
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));

    if (node == NULL)
    {
        printf("�޸� �Ҵ� ����\n");
        exit(-1);
    }

    node->left_child = left;
    node->right_child = right;

    return node;
}

int DestroyTree(TreeNode* p)
{//Ʈ�� ������ ���� ���� �Ҵ�� ��� �ڿ��� ȸ��
    if (p == NULL) return -1;

    DestroyTree(p->left_child);
    DestroyTree(p->right_child);

    free(p);

    return 1;
}

Element HuffmanTree(AlphaType* pArr, int n, Element e)
{//MinHeap�� ���� ���ĵ� Node�� HuffmanTree�� ���ʴ�� ����
    TreeNode* node, * temp;
    Element e1, e2;
    HeapType heap;
    int i;

    heap.heap_size = 0;

    for (i = 0; i < n; i++)
    {
        node = MakeNode(NULL, NULL);
        node->weight.alpha = pArr[i].alpha;
        e.key = node->weight.freq = pArr[i].freq;
        e.pTree = node;
        InsertHeap(&heap, e); // ������ ��带 ���� ���� 
    }

    for (i = 0; i < n - 1; i++) // n-1���� �ݺ�, ������ ���� ��尡 ��Ʈ 
    {
        e1 = DeleteHeap(&heap); // �ּҰ��� ������ ��� 2���� ������ 
        e2 = DeleteHeap(&heap);

        temp = MakeNode(e1.pTree, e2.pTree); // 2���� ��带 ������ ��� ���� 

        e.key = temp->weight.freq = e1.key + e2.key; // 2���� ��� ���� ���Ѵ� 
        e.pTree = temp; // ������ ���� ��带 ���� 

        InsertHeap(&heap, e); // Ʈ���� ������ ��带 ���� ���� 
    }
    e = DeleteHeap(&heap); // ���⼭ ���� �����ʹ� �ϼ��� Ʈ�� 

    return e;//Ʈ�� ��ȯ
}

int GLOBAL = 0;
int BuildTable(TreeNode* p, int i, char* pCode, HuffmanTable* ht)
{
    if (p)
    {
        i++;
        pCode[i] = '1';
        BuildTable(p->left_child, i, pCode, ht);
        pCode[i] = '0';
        BuildTable(p->right_child, i, pCode, ht);
        pCode[i] = '\0';

        if (p->left_child == NULL && p->right_child == NULL)
        {
            ht[GLOBAL].alpha = p->weight.alpha;
            strcpy_s(ht[GLOBAL].code, strlen(pCode) + 1, pCode);
            GLOBAL++;
        }
    }
    return GLOBAL;
}

int main()
{
    AlphaType data[ALPHABET]; // ���ڿ��� ���� ����
    AlphaType* copyData; // �Է¹��� ���ڿ��� ���� ����

    Init(data);

    FILE* fp = NULL;
    char* str = NULL;

    int i, j, k = 0;
    int flag, count = 0;

    if (fopen_s(&fp, "input.txt", "r") == 0) {//���� �� ���ڿ� �б�
        char buffer[MAX_LEN] = { 0, };

        fread(buffer, 1, MAX_LEN, fp);
        str = buffer;

        fclose(fp);
    }
    printf("Input String : %s", str);

    for (i = 'A'; i <= 'Z'; i++)
    {
        flag = 0;
        for (j = 0; j < strlen(str); j++)
        {
            if (i == str[j])
            {
                data[i - 65].freq++;
                flag = 1; // i���� ���ĺ��� ����
            }
        }
        if (flag == 1)
            count++; // ���ĺ��� �����ϹǷ� ī���� ���� 
    }

    copyData = (AlphaType*)malloc(sizeof(AlphaType) * count);

    for (i = 0; i < ALPHABET; i++)
    {
        if (data[i].freq != 0) // �����ϴ� ���ڸ� ���� 
        {
            copyData[k].alpha = i + 65;
            copyData[k].freq = data[i].freq;
            k++;
        }
    }

    printf("\n- character frequency -\n");
    for (i = 0; i < count; i++)
        printf("  %c         %d \n", copyData[i].alpha, copyData[i].freq);

    Element e = {0,};
    e = HuffmanTree(copyData, count, e);
    char pCode[ALPHABET];

    HuffmanTable* ht = (HuffmanTable*)malloc(sizeof(HuffmanTable)* ALPHABET);
    int htCount = 0;
    for (i = 0; i < ALPHABET; i++) {//������ ���̺� ���� �ʱ�ȭ
        initHuffmanTable(&ht[i]);
    }

    htCount = BuildTable(e.pTree, -1, pCode, ht);//������ ���̺� ���� �� ũ�� ��ȯ
    DestroyTree(e.pTree);//Ʈ�� ����

    printf("\n- Huffman Table -\n");
    for (i = 0; ht[i].alpha != '\0'; i++) {
        printf("  %c       %s\n", ht[i].alpha, ht[i].code);
    }

    if (fopen_s(&fp, "output.txt", "w") == 0) {//������ ����
        for (i = 0; i < strlen(str); i++) {
            for (j = 0; ht[j].alpha != '\0'; j++) {
                if (str[i] == ht[j].alpha) {
                    fputs(ht[j].code, fp);
                    break;
                }
            }
        }
        fclose(fp);
    }

    if (fopen_s(&fp, "output.txt", "r") == 0) {//���� ���� �б�
        char buffer[MAX_LEN] = { 0, };

        fread(buffer, 1, MAX_LEN, fp);
        str = buffer;

        fclose(fp);
    }

    char* code = (char*)malloc(sizeof(char)*MAX_LEN);
    code[0] = str[0];
    code[1] = '\0';

    if (fopen_s(&fp, "decode.txt", "w") == 0) {//������ ��������
        for (i = 0; i < strlen(str); i++) {
            for (j = 0; j < htCount; j++) {
                if (strcmp(code, ht[j].code) == 0) {
                    char tmp[2];
                    tmp[0] = ht[j].alpha;
                    tmp[1] = '\0';
                    fputs(tmp, fp);
                    break;
                }
            }
            if (j == htCount) {//��ġ�ϴ� �ڵ尡 ���ٸ� ���� ��Ʈ���� �б�
                strncat_s(code, sizeof(code), &str[i + 1], 1);
            }
            else {//��ġ�ϴ� �ڵ��� �ʱ�ȭ
                code[0] = str[i + 1];
                code[1] = '\0';
            }
        }
        fclose(fp);
    }
    return 0;
}