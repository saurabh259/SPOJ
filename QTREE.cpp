/*
-------------------------------------------------------------------------------
Author: Saurabh Joshi
DataStructure / Algo :  Heavy Light Decomposition , Segment Tree , Binary Tree
-------------------------------------------------------------------------------
*/

#include<cstdio>
#include<vector>
#include<cstring>
#include<cmath>
#include<cstdlib>
#define foreach(v, c)  for( typeof( (c).begin()) v = (c).begin();  v != (c).end(); ++v)
#define Max(a,b)	    (a)>(b)?(a):(b)	

using namespace std;

typedef vector<int> VI;

/* Trying HLD for the first time*/
/* Create segment tree for a heavy chain
   Store info of last white and black node in a heavy chain
   Check query and update */
   
const int maxn = 100005;
VI adjList[maxn];
int color[maxn];
int dad[maxn], subtree[maxn], level[maxn];
int direct_white[maxn],direct_black[maxn];
int N;
int temp[maxn];    //for storing values to be put in ST

struct ST
{
	
	int size,*W,*B;
	
	ST(int N)
	{
		int x=ceil(log2(N)+1);
		size=3*pow(2,x);
		W=new int[size];
		B=new int[size];
		memset(W,-1,sizeof (W));
		memset(B,-1,sizeof (B));

	}
	
	void buildST(int node,int l , int r , int ar[])
	{
		if(l==r)
		{
			if( color[ar[l]]==0)
			{
				
				B[node]=-1;
				W[node]=r;
			}
			else
			{
				
			 	B[node]=r;
			 	W[node]=-1;
			}
			
			
		}
			
			
		
		else 
		{
			buildST(2*node , l , (l+r)/2 , ar);
			buildST(2*node+1 , ((l+r)/2)+1  , r , ar);
			W[node]= Max( W[2*node],W[2*node+1] );
			B[node]= Max( B[2*node],B[2*node+1] );
		}
	}
	
	
	int querySTW(int node, int i,int j, int l, int r)
	{
		if(j<l || i>r) return -10;
		
		if( l>=i && r<=j)
		return W[node];
		
		int a,b;
		a=querySTW(2*node,i,j,l,(l+r)/2);
		b=querySTW(2*node+1,i,j,(l+r)/2+1,r);
		
		if(a==-10)
		return b;
		
		if(b==-10)
		return a;
		
		else return Max(a,b);
		
	}
	
	int querySTB(int node, int i,int j, int l, int r)
	{
		if(j<l || i>r) return -10;
		
		if( l>=i && r<=j)
		return B[node];
		
		int a=querySTB(2*node,i,j,l,(l+r)/2) ;
		int b=querySTB(2*node+1,i,j,(l+r)/2+1,r);
		
		if(a==-10)
		return b;
		
		if(b==-10)
		return a;
		
		else return Max(a,b);
	}
	
	void updateST(int node,int i, int l,int r)
	{
		if(i<l || i>r) return ;
		
		if( l==r )
		{
			if(W[node]==-1)
			{
			
				W[node]=l;
				B[node]=-1;
			}
			else
			{
			
				B[node]=l;
				W[node]=-1;
			}
		}
		
		else
		{
			updateST(2*node ,i, l , (l+r)/2 );
			updateST(2*node+1 ,i, ((l+r)/2)+1  , r );
			W[node]= Max( W[2*node],W[2*node+1] );
			B[node]= Max( B[2*node],B[2*node+1] );
		}
	}
};

struct BIT
{
	long long int *BITW;
	long long int  *BITB;
	int n;
	BIT(int N)
	{
		n=N;
		BITW=new long long int[n+2];
		BITB=new long long int[n+2];
		
		for(int i=0;i<=n;i++)
		BITW[i]=BITB[i]=0;

	}
	
	inline void updateW(int idx,long long  val)
	{
		while(idx<=n)
		{
		

			BITW[idx]+=val;
			
		
			idx+=(idx & -idx);
			
		}
	}
	
	inline void updateB(int idx,long long val)
	{
		while(idx<=n)
		{
			BITB[idx]+=val;
			idx+=(idx & -idx);
		}
	}
	
	long long queryB(int x)
	{
		long long  ans=0;
		int i=x;
		
		
		while(i>0)
		{
			ans+=BITB[i];
			i-=(i&-i);
		}		
		return ans;
	}
	
	 long long  queryW(int x)
	{
		long long ans=0;
		int i=x;
		
		
		while(i>0)
		{
			ans+=BITW[i];
			i-=(i&-i);
		}		
		return ans;
	}
};
	 

	
				
			


//Rooting tree at 1 and running DFS to know size of each sub-tree
int dfs(int u, int p)
{
  if(dad[u] >= 0) return 0;
  dad[u] = p;
  
  
  int its = 0;
  foreach(it, adjList[u])
    {
		if(dad[*it]==-1)
		its += dfs(*it, u);
	
	}
//	printf("Subtree size of %d is :%d \n",u,1+its);
	
	direct_white[u]=0;
	direct_black[u]=its;
  return subtree[u] = 1 + its;
  			
}

int cnum;
VI chain[maxn];
int chainHead[maxn], chainPos[maxn], chainIndex[maxn];


// Decomposing tree into chains using HLDoT
void HLD(int u)
{
//			printf("HLD debug1\n");

  if(chain[cnum].size() == 0)
    chainHead[cnum] = u;
  
  chain[cnum].push_back(u);
  chainPos[u] = chain[cnum].size() - 1;
  chainIndex[u] = cnum;
  
  int most = 0, ind = -1;
  foreach(it, adjList[u])
  {
    int v = *it;
    if(v == dad[u]) continue;
    if(subtree[v] > most)
      most = subtree[v], ind = v;

  }
  if(ind >=0) HLD(ind);
  
  foreach(it, adjList[u])
  {
    int v = *it;
    if(v == dad[u] || v == ind) continue;
    ++cnum;
    HLD(v);

  }
  	

}

vector<ST> pathST;
vector<BIT>pathBIT;



void Tree_query( int node )
{
	int index, node_chain_no,node_pos,ans_index=0,ans_cno=0,ans_npos=0;
	int node_color=color[node];
	while(true)
	{
		node_chain_no=chainIndex[node];
		node_pos=chainPos[node];
		
		if(node_color==0)
		 index=pathST[node_chain_no].querySTB(1,0,node_pos,0,chain[node_chain_no].size()-1);
		else
		 index=pathST[node_chain_no].querySTW(1,0,node_pos,0,chain[node_chain_no].size()-1);

		index++;
		if(index-1==node_pos)  //invalid node in other chain
		break;
		
		ans_index=index;
		ans_cno=node_chain_no;
		ans_npos=node_pos;
				
		node= dad[ chainHead[node_chain_no] ] ;
		if(node==-1 || index!=0)
		break;
	}
	
	long long ans=0;
	if(!node_color)
	{
		ans=pathBIT[ans_cno].queryW(ans_index+1);
		ans+=direct_white[ chain[ans_cno][ans_index] ];
	}
	else
	{
		ans=pathBIT[ans_cno].queryB(ans_index+1);
		ans+=direct_black[ chain[ans_cno][ans_index] ];
	}
	printf("%lld\n",ans+1);

	
	
}

void Tree_update(int node)
{
	
	int index, node_chain_no=chainIndex[node],node_pos=chainPos[node],ans_index=0,orig=node,sub;
	int node_color=color[node];
	
	if(!node_color)
	{
		sub=pathBIT[node_chain_no].queryW(node_pos+1);
		 
		sub+=direct_white[node];
	}

	else if(node_color)
	{
		sub=pathBIT[node_chain_no].queryB(node_pos+1);
		 sub+=direct_black[node];
	}
	
	sub++;
	int chain_upd=0;
	int flag=0;
	int offset=0;
	while(true)
	{
		offset=0;
		chain_upd++;
		node_chain_no=chainIndex[node];
		node_pos=chainPos[node];
		
		if(node_color==0)
		 index=pathST[node_chain_no].querySTB(1,0,node_pos,0,chain[node_chain_no].size()-1);
		else
		 index=pathST[node_chain_no].querySTW(1,0,node_pos,0,chain[node_chain_no].size()-1);
	
		index++;
		
		if(index==0) //no black node found
		offset=1;
		if(index-1==node_pos)
		{
			flag=1;
			
		}
		
		if( !node_color)
		{
		
			pathBIT[node_chain_no].updateW(index+offset,-sub);
		
			pathBIT[node_chain_no].updateW(node_pos+1+(chain_upd>1),sub);
		
		}
		else
		{
		
			pathBIT[node_chain_no].updateB(index+offset,-sub);
			pathBIT[node_chain_no].updateB(node_pos+1+(chain_upd>1),sub);
		}
		
		index--;
		node= dad[ chainHead[node_chain_no] ] ;
		if( index!=-1 || node==-1 || flag==1)
		break;
		
	}
	
	node=orig;
	color[node]=!node_color;
	pathST[ chainIndex[node] ] .updateST(1, chainPos[node],0,chain[chainIndex[node]].size()-1);
	
	int add;
	
	node_chain_no=chainIndex[node],node_pos=chainPos[node];
	node_color=color[node];
	
	if(!node_color)
	{
		add=pathBIT[node_chain_no].queryW(node_pos+1);
		
		add-=pathBIT[node_chain_no].queryW(node_pos);
		add+=direct_white[node];
	}
	
	else if(node_color)
	{
		add=pathBIT[node_chain_no].queryB(node_pos+1);
		add+=direct_black[node];

	}
	
	add++;
	chain_upd=0;
	flag=0;
	while(true)
	{
		offset=0;
		chain_upd++;
		node_chain_no=chainIndex[node];
		node_pos=chainPos[node];
		
		if(node_color==0)
		 index=pathST[node_chain_no].querySTB(1,0,node_pos,0,chain[node_chain_no].size()-1);
		else
		 index=pathST[node_chain_no].querySTW(1,0,node_pos,0,chain[node_chain_no].size()-1);
	
		index++;
		if(index==0)
		offset=1;
		if(index-1==node_pos)
		{
			flag=1;
		}
		
		if( !node_color)
		{
		
			pathBIT[node_chain_no].updateW(index+offset,add);
			pathBIT[node_chain_no].updateW(node_pos+1+(chain_upd>1),-add);
		}
		else
		{
		
			pathBIT[node_chain_no].updateB(index+offset,add);
			pathBIT[node_chain_no].updateB(node_pos+1+(chain_upd>1),-add);
		}
		
		index--;
		node= dad[ chainHead[node_chain_no] ] ;
		if( index!=-1  || node==-1 || flag==1)
		break;
		
	}

	
}	

int main()
{
	//freopen("in.txt","r",stdin);
	int n,x,y;
	scanf("%d",&n);
	
	for(int i=0;i<n-1;i++)
	{
		scanf("%d%d",&x,&y);
	
		adjList[x].push_back(y);
		adjList[y].push_back(x);
		dad[i+1]=-1;
		color[i+1]=1;
	}
	dad[n]=-1;
	color[n]=1;
	
	dfs(1,n);
	dad[1]=-1;
	
	cnum=0;
	HLD(1);
	cnum++;
	
	
	
	//Creating BIT and ST for paths
	for(int c=0;c<cnum;c++)
   	{
     int k=0;
  
     
     BIT temp_BIT ( chain[c].size() );
     foreach(it, chain[c])
     {
	   	temp[k++]=*it;     

	 }
	 
	   pathBIT.push_back(temp_BIT);
	   ST temp_tree(k);
	   temp_tree.buildST(1,0,k-1,temp);
       pathST.push_back(temp_tree);

   }
   

    int q,a,b;
    scanf("%d",&q);
    for(int i=0;i<q;i++)
    {
    	scanf("%d %d",&a,&b);
    	if(a==0)
    	Tree_query(b);
    	else
    	{
    		Tree_update(b);
		} 	
    }
}
		
