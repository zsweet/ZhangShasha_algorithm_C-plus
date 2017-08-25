#include <bits/stdc++.h>
#include "Tree.h"
#include "mynode.h"

//#include  "/usr/include/x86_64-linux-gnu/sys/io.h"
//#include <windows.h>
using namespace std;

int TD[MAXNODE][MAXNODE];

int ZhangShasha(Tree &tree1, Tree &tree2);
int treedist(vector <int> left1, vector <int> left2, int i, int j, Tree tree1, Tree tree2);
void getmodifycost(Tree tree1,Tree tree2);
vector <Node> test2(int &root);
vector <Node> test1(int &root);
void test();



int main()
{

    test();

    return 0;
}


void printfinfo(Tree tree)
{
    cout<<"==========节点信息=============\n nodename index leftmost child_number\n";
    for(int i=0; i<tree.nodelist.size(); i++)
        //if(tree)
        cout<<tree.nodelist[i].nodename<<' '<<tree.nodelist[i].index<<' '<<tree.nodelist[i].leftmost<<' '<<tree.nodelist[i].edgelist.size()<<endl;

    cout<<"\n=============最左节点信息==============\n";
    for(int i=0; i<tree.left.size(); i++)
        cout<<tree.left[i]<<' ';

    cout<<"\n=============keyroots点信息==============\n";
    for(int i=0; i<tree.keyroots.size(); i++)
        cout<<tree.keyroots[i]<<' ';

    cout<<"\n============提取的节点名称信息==============\n";
    for(int i=0; i<tree.labels.size(); i++)
        cout<<tree.labels[i]<<' ';


    cout<<"\n\n\n";
    return ;
}

int ZhangShasha(Tree &tree1, Tree &tree2)
{

 //   cout<<"debug :come in Zhang-shasha \n";
    tree1.getindex();
    //cout<<"debug : tree1 getindex is over \n";
    //return 0;
  //  cout<<"debug :finish tree1 getindex \n";
    tree1.getleftarr();
//   cout<<"debug :finish tree1 getleftarr \n";
    tree1.getkeyroots();
    tree1.traverse();
  //  cout<<"debug :finish tree1 initial \n";

    tree2.getindex();
    tree2.getleftarr();
    tree2.getkeyroots();
    tree2.traverse();
  //  cout<<"debug :initial is over \n";
    printfinfo(tree1);
    printfinfo(tree2);

    vector <int> left1 = tree1.left;
    vector <int> keyroots1 = tree1.keyroots;

    vector <int> left2 = tree2.left;
    vector <int> keyroots2 = tree2.keyroots;
    //cout<<keyroots1.size()<<" "<<keyroots2.size()<<endl;
    // space complexity of the algorithm


    getmodifycost(tree1,tree2);

  //  cout<<"getmodifycost is over!!!!\n";
    // solve subproblems
    for (int i1 = 1; i1 < keyroots1.size() + 1; i1++)           ///对Tree1的LR_keyroots集合遍历
    {
        for (int j1 = 1; j1 < keyroots2.size() + 1; j1++)       ///对Tree2的LR_keyroots集合遍历
        {
            int i = keyroots1[i1 - 1];
            int j = keyroots2[j1 - 1];
            TD[i][j] = treedist(left1, left2, i, j, tree1, tree2);
           // cout<<i<<" "<<j<<" "<<TD[i][j]<<endl;
        }
    }
    return TD[left1.size()][left2.size()];
}

int forestdist[MAXNODE][MAXNODE];  ///???????????????????
int delcost1[MAXNODE],relcost[MAXNODE][MAXNODE];
int inscost2[MAXNODE];

void getmodifycost(Tree tree1,Tree tree2)
{
    int Delete = 1;
    int Insert = 1;
    int Relabel = 1;
    int len1=tree1.labels.size(),len2=tree2.labels.size();
    for(int i=0; i<len1; i++)
        delcost1[i]=Delete;

    for(int i=0; i<len2; i++)
        inscost2[i]=Insert;

    for(int i=0; i<len1; i++)
       for(int j=0; j<len2; j++)
        relcost[i][j] = Relabel;
}

int treedist(vector <int> left1, vector <int> left2, int i, int j, Tree tree1, Tree tree2)
{
//   int **forestdist = new int*[i + 1];
//   for(int k=0;k<i+1;k++)
    //      forestdist[k]=new int[j + 1];


    // costs of the three atomic operations

    forestdist[0][0] = 0;
    for (int i1 = left1[i-1]; i1 <= i; i1++)
    {
        forestdist[i1][0] = forestdist[i1 - 1][0] + delcost1[i1];
    }
    for (int j1 = left2[j - 1]; j1 <= j; j1++)
    {
        forestdist[0][j1] = forestdist[0][j1 - 1] + inscost2[j1];
    }
    for (int i1 =  left1[i-1]; i1 <= i; i1++)
    {
        for (int j1 = left2[j - 1]; j1 <= j; j1++)
        {
            int i_temp = (left1[i - 1] > i1 - 1) ? 0 : i1 - 1;
            int j_temp = (left2[j - 1] > j1 - 1) ? 0 : j1 - 1;
            if ((left1[i1 - 1] == left1[i - 1]) && (left2[j1 - 1] == left2[j - 1]))  ///i在i1的左子树上
            {
                int Cost = (tree1.labels[i1 - 1]==tree2.labels[j1 - 1]) ? 0 : relcost[i1][j1];
                forestdist[i1][j1] = min(min(forestdist[i_temp][j1] + delcost1[i1], forestdist[i1][j_temp] + inscost2[j1]),
                                         forestdist[i_temp][j_temp] + Cost);
                TD[i1][j1] = forestdist[i1][j1];
               // if(TD[i1][j1]!=0)
               //     printf("debug: TD[%d][%d]=%d\n",i1,j1,TD[i1][j1]);
                // println(i1 + " " + j1);
            }
            else   ///i在i1的右子树上 ，因此需要分割成两部分计算
            {
                int i1_temp = left1[i1 - 1] - 1;
                int j1_temp = left2[j1 - 1] - 1;

                int i_temp2 = (left1[i - 1] > i1_temp) ? 0 : i1_temp;
                int j_temp2 = (left2[j - 1] > j1_temp) ? 0 : j1_temp;

                forestdist[i1][j1] = min(min(forestdist[i_temp][j1] + delcost1[i1], forestdist[i1][j_temp] + inscost2[j1]),
                                         forestdist[i_temp2][j_temp2] + TD[i1][j1]);
            }
        }
    }
    /* for(int k=0;k<i+1;k++)
      delete[] forestdist[k];
    delete[] forestdist;*/
   // System.out.println("########################");
    return forestdist[i][j];
}


void test()
{
    vector<Node> nodeL1; ///树节点信息
    vector<Node> nodeL2; ///树节点信息
    int root1,root2;

    nodeL1 = test1(root1),nodeL2=test2(root2);

    Tree tree1(nodeL1,root1),tree2(nodeL2,root2);
    int distance = ZhangShasha(tree1,tree2);
    printf("the distance between tree1 and tree2 is :%d",distance);
}

vector <Node> test1(int & root)
{
    root=0;
    vector<Edge> edge;
    vector <Node> node;
    ///0
    edge.clear();
    edge.push_back(Edge(1));
    edge.push_back(Edge(2));
    node.push_back(Node(edge,"f"));

    ///1
    edge.clear();
    edge.push_back(Edge(3));
    edge.push_back(Edge(4));
    node.push_back(Node(edge,"d"));

    ///2
    edge.clear();
    node.push_back(Node(edge,"e"));

    ///3
    edge.clear();
    node.push_back(Node(edge,"a"));

    ///4
    edge.clear();
    edge.push_back(Edge(5));
    node.push_back(Node(edge,"c"));

    ///5
    edge.clear();
    node.push_back(Node(edge,"b"));
    return node;
}


vector <Node> test2(int &root)
{
    root=0;
    vector<Edge> edge;
    vector <Node> node;
    ///0
    edge.clear();
    edge.push_back(Edge(1));
    edge.push_back(Edge(2));
    node.push_back(Node(edge,"f"));

    ///1
    edge.clear();
    edge.push_back(Edge(3));
    node.push_back(Node(edge,"c"));

    ///2
    edge.clear();
    node.push_back(Node(edge,"e"));

    ///3
    edge.clear();
    edge.push_back(Edge(4));
    edge.push_back(Edge(5));
    node.push_back(Node(edge,"d"));

    ///4
    edge.clear();
    node.push_back(Node(edge,"a"));

    ///5
    edge.clear();
    node.push_back(Node(edge,"b"));
    return node;

}
