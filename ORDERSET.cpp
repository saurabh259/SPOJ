/*
-------------------------------------------------------------------------------
Author: Saurabh Joshi
DataStructure / Algo :  Red Black Tree
-------------------------------------------------------------------------------
*/



#include<stdio.h>
#include<malloc.h>
#define mod 1000000007
#include<deque>
using namespace std;
typedef struct node{
        int  key;
        struct node *left;
        struct node *right;
        int color;
        struct node *parent;
         int lc;
         int rc;
        
        ~node()
        {
   free (left);
   free (right);
          }
        
} node;

class rbtree{
      

public :
node *root;
node *nill;
rbtree(){
         
         nill=(node *)malloc(sizeof(node));
         nill->key=-1000000002;
         nill->left=NULL;
         nill->right=NULL;
         nill->color=0;
         nill->parent=NULL;
         nill->lc=nill->rc=0;
         root=nill;
         }
~rbtree(){
         delete root;}
//void treeInsert(struct node*,int val);
//void rbFixup(struct node *);
//void leftRotate(struct node*);
//void rightRotate(struct node*);

void rightRotate(node* z )
{
    
     
    node* x=z->left;
     
     z->left=x->right;
      if(x->right!=nill)
     x->right->parent=z;
     x->right=z;
     x->parent=z->parent;
     if(z->parent!=nill){
     if(z==z->parent->left)
     z->parent->left=x;
     else
     z->parent->right=x;}
     else root=x;
     z->parent=x;
     
     //Changing left and right count
     int temp=x->rc;
     x->rc+=1+z->rc;
     z->lc=temp;
}

void leftRotate(node* z )
{
     
     
     node * x=z->right;
     
     z->right=x->left;
     if(x->left!=nill)
     x->left->parent=z;
     x->left=z;
     x->parent=z->parent;
     if(z->parent!=nill){
     if(z==z->parent->left)
     z->parent->left=x;
     else
     z->parent->right=x;}
     else root=x;
     z->parent=x;
     
     //Changing left and right count
     int temp=x->lc;
     x->lc+=1+z->lc;
     z->rc=temp;
}
     


void rbInsertfixup(node *z){
     
     while(z->parent->color==1){
                                
                                
                               // printf("Fixing node %d\n\n",z->key);
                                node *y=nill;
                                //Finding Uncle of z
                                if(z->parent==z->parent->parent->left)
                                y=z->parent->parent->right;
                                else
                                y=z->parent->parent->left;
                                
                                //Case 1
                                if(y!=nill && y->color==1 ){
                                y->color=0;
                                z->parent->color=0;
                                z->parent->parent->color=1;
                                z=z->parent->parent;
                              //  printf("**Debug\n");
                               }
                               
                               
                               else if(z->parent==z->parent->parent->left){
                                 //Case 2
                                 if(z==z->parent->right){
                                                         z=z->parent;
                                                         leftRotate(z);
                                 } 
                                 // Case 3                        
                                 z->parent->color=0;
                                 z->parent->parent->color=1;
                                 rightRotate(z->parent->parent);
                               
                               }
                               //Symmetric else part
                               else{
                                    //Case 2
                                 if(z==z->parent->left){
                                                         z=z->parent;
                                                         rightRotate(z);
                                 } 
                                 // Case 3                        
                                 z->parent->color=0;
                                 z->parent->parent->color=1;
                                 leftRotate(z->parent->parent);
                                 }
                  
                  
                  if(z==root)
                  {
                             z->color=0;
                             break;
                  }
     }
     root->color=0;
   //  if(z->parent!=NULL)
     //printf("Left count :%d\n Right count:%d\n",z->parent->lc,z->parent->rc);
     
}     

void treeInsert(int val){
     int flag=1;
     //Insert root node
     if(root==nill){
                   root=(node *)malloc(sizeof(node));
                   root->key=val;
                   root->left=root->right=root->parent=nill;
                   root->lc=root->rc=0;
                   root->color=0;
                  
                   
                   
                   }
     
     else{
          node *x=root,*y=NULL;
          
          //Search for right location
          while(x!=nill){
                         y=x;
                         if(val>x->key)
                         {
                                       x->rc+=1;
                                       x=x->right;
                         }
                         else 
                         {
                             x->lc+=1;
                             x=x->left;
                             
                         }
                        
          }
           
           //Create new node
           x=(node *)malloc(sizeof(node));
           x->key=val;
           x->left=x->right=nill;
           x->parent=y;
           if(x->key > y->key)
           y->right=x;
           else
           y->left=x;
           x->lc=x->rc=0;
           x->color=1;
           rbInsertfixup(x);
          }
          
               
     
}
     
void rbTransplant(node *u,node *v){
     
     if(u->parent==nill)
       root=v;
     
     else if(u==u->parent->left)
      u->parent->left=v;

     else if(u==u->parent->right)
      u->parent->right=v;
      

v->parent=u->parent;
     
}

void rbDeletefixup(node *x){
     if(x->parent==NULL)
     x=root;
    // printf("Fixing up node *** %d\n",x->key);
  //   printf("x value %d***\n",x->parent->left->key);
     node *w=nill;
     while(x!=root && x->color==0){
                   if(x==x->parent->left)
                   {
                                         w=x->parent->right;
                                         // printf("w value %d***\n",w->key);
                                         // printf("w children color %d__%d\n",w->left->color,w->right->color);
                                          
                                          if(w==nill)
                                          x=x->parent;
                                         //Case 1
                                         else{
                                         if(w->color==1)
                                         {
                                                        w->color=0;
                                                        x->parent->color=1;
                                                        leftRotate(x->parent);
                                                        w=x->parent->right;
                                         }
                                         
                                         //Case 2
                                     //    printf("w  %d__%d\n",w->key,w->key);
                                         if(w->right->color==0 && w->left->color==0)
                                         {
                                                               w->color=1;
                                                               x=x->parent;
                                         }
                                         
                                         //Case 3
                                         else if(w->right->color==0)
                                         {
                                              w->left->color=0;
                                              w->color=1;
                                              rightRotate(w);
                                              w=x->parent->right;
                                         }
                                         
                                         //Case 4
                                         
                                         if(w->right->color==1){
                                         //printf("Right");
                                         w->color=x->parent->color;
                                         x->parent->color=0;
                                         w->right->color=0;
                                         leftRotate(x->parent);
                                         x=root;
                                         }
                   }
                   }
                   
                   if(x==x->parent->right)
                   {
                                         w=x->parent->left;
                                       //  printf("W RIGHT COLOR %d\n",w->right->color);
                                         //Case 1
                                         if(w->color==1)
                                         {
                                                        w->color=0;
                                                        x->parent->color=1;
                                                        rightRotate(x->parent);
                                                        w=x->parent->left;
                                         }
                                         
                                         //Case 2
                                         if(w->right->color==0 && w->left->color==0)
                                         {
                                                               w->color=1;
                                                               x=x->parent;
                                         }
                                         
                                         //Case 3
                                         else if(w->left->color==0)
                                         {
                                             
                                              w->right->color=0;
                                              w->color=1;
                                              leftRotate(w);
                                              w=x->parent->left;
                                         }
                                         
                                         //Case 4
                                         if(w->left->color==1){
                                         w->color=x->parent->color;
                                         x->parent->color=0;
                                         w->left->color=0;
                                         rightRotate(x->parent);
                                         x=root;
                                         }
                   }

     }
     x->color=0; 
}
void treeDelete(node* z){
     
    // printf("Deleting.... node %d\n",z->key);
     node *y=z,*x=nill;
     int y_orig_color=y->color;
     
     
     if(z->left==nill)
     {
       //printf("CASE 1\n");
        x=z->right;
        if(x==nill)
        x->parent=z;
        rbTransplant(z,x);
     }
     else if(z->right==nill)
     {
        //printf("CASE 2\n");
        x=z->left;
        rbTransplant(z,x);
     }
     
     else{
          
       //   printf("CASE 3\n");
          y=z->right;
          x=y;
          //Finding Successor of z
          while(y!=nill){
                         x=y;
                         y=y->left;
                         x->lc--;
                         }
          y=x;
          x=y->right;
        //  printf("succ %d\n",y->key);
          //printf("y value %d \n",y->key);
          y_orig_color=y->color;
          if(y->parent==z)
            x->parent=y;
                           
          else{
               rbTransplant(y,x);
               y->right=z->right;
               y->rc=z->rc-1;
               y->right->parent=y;
               }
          rbTransplant(z,y);
          y->left=z->left;
          y->lc=z->lc;
          y->left->parent=y;
          y->color=z->color;
          }
          free(z);
          
          if(y_orig_color==0)
          rbDeletefixup(x);
          
     }
                         
          
                
                        
int exist(int val){
    node *q=root;
    
    while(q!=nill)
    {
                  if(q->key==val)
                  return 1;
                                                      
                  if(q->key>val)
                  q=q->left;
                  
                  else q=q->right;
    }
    return 0;
}
                       
     
     
 node * korder( int k){
    
    node *y=root;
   
    while(true){
                
             //printf("%d is root value with l and r \n: %d %d\n",y->key,y->lc,y->rc);
                if(y==nill)
                {
                           
                           return y;
                }
                
                
                if((y->lc+1)==k)
                return y;
    
                else if(y->lc+1<k)
                {
                     k-=y->lc+1;
                     y=y->right;
                     
                }
                else {
                     y=y->left;
                     }
                }
}
    
    
};  



int main()
{
    int a,tc,b;
    
    char ch,buff;
    rbtree t;
    
    
    scanf("%d",&tc);
    
    while(tc--)
    {
      
      scanf("%c",&buff);        
  
    scanf("%c",&ch);
    
   
    
    if(ch==73)
    {
            scanf("%d",&a);
            if(!t.exist(a))
            t.treeInsert(a);
            
            
    }
    
    else if(ch==68)
    {
            scanf("%d",&a);
            
            if(t.exist(a)){
            node *q=t.root;
            while(q!=t.nill)
            {
                        
                       
                       if(q->key==a)
                       break;
                    
                       if(q->key>a)
                       {
                                   q->lc-=1;
                                   q=q->left;
                       }
                       else
                       {
                           q->rc-=1;
                           q=q->right;
                       }
            }
         
            t.treeDelete(q);
            }
                }
   
    else if(ch==75) {
                          scanf("%d",&b);
                         // fflush(stdin);
                          node * v=t.korder(b);
                          if(v==t.nill)
                          printf("invalid\n");
                          
                          else
                          printf("%d\n",v->key);
                         // fflush(stdout);
                         
               }
    
    
   else if(ch==67)  { 
                  int count=0;
                 scanf("%d",&a);
              
                 node *q=t.root;
                 while(q!=t.nill)
                 {
                       if(q->key==a)
                       {
                                    count+=q->lc;
                                    break;
                       }
                       else if(q->key>a)
                       q=q->left;
                       else
                       {
                        
                           count+=q->lc+1;
                           q=q->right;
                       }
            }
            printf("%d\n",count);
         
            
            }
            

   
}
    return 0;
}

