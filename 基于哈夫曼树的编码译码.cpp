
#include <stdio.h>
#include<string.h>
#include<malloc.h>


#define MAX_NODE 200
#define FALSE -1
#define ASCLLup 126
#define ASCLLlow 32
#define charlen 20


//----赫夫曼树和赫夫曼树编码的存储表示----
typedef struct {
    char data;
    unsigned int weight;
    unsigned int parent, lchild, rchild;
}HTNode, * HuffmanTree;//动态分配数组存储赫夫曼树

typedef char ** HuffmanCode;//动态分配数组存储赫夫曼编码表

//----赫夫曼编码算法----



void Select(HuffmanTree& HT, int n, int& s1, int& s2) {
    //在HT[1]~HT[i-1]中选择parent为0且weight最小的节点，序号分别为s1,s2
    
    for (int i = 1; i < n; i++) {         //初始化s1，s1的双亲为0
         if (HT[i].parent == 0) {     
                s1 = i;					  //现寻找到一个双亲为0的节点作为比较基准，再遍历寻找权值最小的点
                break;                        
         }
    }
    for (int i = 1; i < n; i++) {         //s1为权值最小的下标
         if (HT[i].parent == 0 && HT[s1].weight > HT[i].weight)
                s1 = i;
        }

    for (int j = 1; j < n; j++) {         //初始化s2，s2的双亲为0
         if (HT[j].parent == 0 && j != s1) {
            s2 = j;
            break;
         }
    }
    for (int j = 1; j < n; j++) {         //s2为另一个权值最小的下标
        if (HT[j].parent == 0 && HT[s2].weight > HT[j].weight && j != s1)
            s2 = j;
    }
}

char HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC, int weight[], int n) {   //n为 leaf个数
    //weight存放n个字符的权值，构造赫夫曼树HT，并求出n个字符的赫夫曼编码HC
    int size = 2 * n - 1, i = 0;         //计算哈夫曼树的节点总数，i为计数器
	int cdlen = 0;                       //初始化哈夫曼编码长度为0

    if (n <= 1) {
        printf("ERROR_02");
        return FALSE;
    }

    for (i = n + 1;i <= size;++i) {      //先对每一个节点初始化为0
        HT[i].weight = 0;
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }

    for (i = n + 1; i <= size; ++i) {   //建子树，根据权重选择最小的两个节点构建新的节点
        int s1 = 0, s2 = 0;
        Select(HT, i, s1, s2);          //改i-1为i
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
    
    //(从根出发）无栈非递归遍历赫夫曼树，求赫夫曼编码
    char *coding = (char*)malloc(sizeof(char)*n);
    if(coding==NULL) return FALSE;
    int current_index = size; //在遍历过程中指示当前处理的节点，从而进行相应的操作，如移动到左孩子节点、右孩子节点或回溯到父节点。
    for (i = 1; i <= size; ++i) {
        HT[i].weight = 0; //结点状态标志：0-根；1-左；2-右
    }
    while (current_index) {
        if (HT[current_index].weight == 0) {//若当前节点为根节点
            HT[current_index].weight = 1;//标志根节点已经访问，下次先进入左孩子
            if (HT[current_index].lchild != 0) {
                current_index = HT[current_index].lchild;
                coding[cdlen++] = '0';//若有左孩子节点，则移至左孩子节点，并将当前节点编码设为0
                
            }
            else if(HT[current_index].rchild==0) {//如果当前节点没有左右孩子，则为叶子结点
                HC[current_index] = (char*)malloc((cdlen) * sizeof(char));//改cdlen+1为cdlen
                if(HC[current_index]==NULL) free(HC[current_index]);
				coding[cdlen] = '\0';
                printf("节点序号: %c,weight：%d  Degree: %d, Level: %d\n", HT[current_index], HT[current_index].weight,(HT[current_index].lchild != 0) + (HT[current_index].rchild != 0), cdlen+1);
                strcpy(HC[current_index], coding);
            }
        }
        else if (HT[current_index].weight == 1) {//如果当前节点为左孩子节点
            HT[current_index].weight = 2;//标志左孩子已经访问
            if (HT[current_index].rchild != 0) {//若右孩子不为0，则向右继续访问
                current_index = HT[current_index].rchild;
                coding[cdlen++] = '1';
            }
        }
        else   //左右子树均已访问过
        {
            HT[current_index].weight = 0;//重置节点状态
            current_index = HT[current_index].parent;//回溯到父节点
            --cdlen;
        }
    }
    free(coding);
}

 //----寻找字符序号----

char* code(HuffmanTree HT, HuffmanCode HC, char* str, int n) {
    char* encodedString = (char*)malloc(n * MAX_NODE * sizeof(char));
    if(encodedString==NULL) free(encodedString);
    *encodedString = '\0';
    int len = 0;  //用来跟踪已编码字符串的长度

    for (int i = 0; i < n; ++i) {
        char c = str[i];    //获取需要寻找的字符
        int index = -1;

        int j = 1;
        while (HT[j].data != c) {
            j++;
        }
        index = j;

        if (index == -1) {    //index仍是-1，即未找到字符的索引
            printf("ERROR_02");
            free(encodedString);
            return NULL;
        }

        strcat(&encodedString[len], HC[index]);
        len += strlen(HC[index]);
    }

    return encodedString;
}

//----哈夫曼译码函数

char* decode(HuffmanTree HT, HuffmanCode HC, char* encodedString, int len2, int num) {
	//len2为待解码字符数组长度。num用作根节点索引

	char* decodedString = (char*)malloc(len2 * sizeof(char));
	if(decodedString==NULL)  free(decodedString);
    int i = 0;//用作计数器用来计算输入的解码字符串
    int j = 0;//跟踪解码字符串长度
    int index = num;

     while (encodedString[i] != '\0') {
         if (encodedString[i] == '0') {//权重为0，代表是左子树，向左遍历
              index = HT[index].lchild;  
              if (HT[index].lchild == 0 && HT[index].rchild == 0) {//叶子结点
                  decodedString[j++] = HT[index].data;//将当前节点数据存入解码后的字符串中
                  index = num;//重新设置为根节点索引，译码下一个字符
              }
         }
         
         //----对于右子树同上操作
         
         else if (encodedString[i] == '1') {
                  index = HT[index].rchild;
                  if (HT[index].rchild == 0 && HT[index].lchild == 0) {
                      decodedString[j++] = HT[index].data;
                      index = num;
                  }
         }
         else {
             printf("ERROR_03");
             free(decodedString);
             return NULL;
         }

        if (index<0 || index>num) {//当前节点索引不在有效范围内
              printf("ERROR_03");
              free(decodedString);
              return NULL;
         }     
         ++i;   
     }   

     if ((HT[index].lchild != 0 || HT[index].rchild != 0) && index != num) {//还有左右子树，即解码完了但是不在叶子结点上
         printf("ERROR_03");
         free(decodedString);
         return NULL;
     }

    decodedString[j] = '\0';
    return decodedString;
}


int main(int argc, char* argv[]){
    if (argc != 3) {
        printf("ERROR_01");
        return -1;
    }

    char *str = argv[1];
    int n = strlen(str);
    if (n < charlen) {
        printf("ERROR_02");
        return -1;
    }

    int charCount[ASCLLup] = { 0 };
    for (int i = 0; i < n; ++i) {
        charCount[(int)str[i]]++; }//计算每个字符出现的次数
    
    
    HuffmanTree HT = (HuffmanTree)malloc((MAX_NODE + 1) * sizeof(HTNode));
    if(HT==NULL)  {free(HT);return FALSE;}
    HuffmanCode HC = (HuffmanCode)malloc((n + 1) * sizeof(char*));
    if(HC==NULL)  {free(HC);return FALSE;}

    // 初始化Huffman树
    int leaf = 1;  // 记录叶子节点的位置
    for (int j = ASCLLlow; j < ASCLLup; ++j) {//ASCLL码32~126
        if (charCount[j] > 0) {//如果该字符在输入字符串中出现过
            HT[leaf].data = (char)j;
            HT[leaf].weight = charCount[j];
            HT[leaf].parent = 0;
            HT[leaf].lchild = 0;
            HT[leaf].rchild = 0;
            leaf++;
        }
    }
    
    HuffmanCoding(HT, HC, charCount, leaf-1);

    char* encodedResult = code(HT, HC, str, n);

    char* decodedResult = decode(HT, HC, argv[2], strlen(argv[2]),2*(leaf-1)-1);

    if (encodedResult != NULL && decodedResult != NULL) {
        printf("%s ", encodedResult);
        printf("%s", decodedResult);
    }

    free(encodedResult);
    free(decodedResult);
    free(HC[1]);
    free(HC[2]);
    free(HT);

    return 0;
}
