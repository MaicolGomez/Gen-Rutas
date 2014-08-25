#include<bits/stdc++.h>
using namespace std;
#define pi (2.0*acos(0.0))
#define eps 1e-6
#define ll long long
#define inf (1<<29)
#define vi vector<int>
#define vll vector<ll>
#define sc(x) scanf("%d",&x)
#define scl(x) scanf("%lld",&x)
#define all(v) v.begin() , v.end()
#define me(a,val) memset( a , val ,sizeof(a) )
#define pb(x) push_back(x)
#define pii pair<int,int> 
#define mp(a,b) make_pair(a,b)
#define Q(x) (x) * (x)
#define L(x) ((x<<1) + 1)
#define R(x) ((x<<1) + 2)
#define M(x,y) ((x+y)>>1)
#define fi first
#define se second
#define MOD 1000000007
#define ios ios::sync_with_stdio(0)
#define N 100000

int main(){
    srand( time(NULL) );
    vector< pii > v;
    for(int i = 0 ; i < 10 ; i++){
        int r = rand() % 5 + 1;
        for(int j = 0 ; j < r ; j++)
            v.pb( mp( rand() , i ) );
    }
    for(int i = 0 ; i < v.size() ; i++)
        cout << v[i].fi << " " << v[i].se << endl;
    
    cout << "-----\n";
    random_shuffle( all(v) );
    for(int i = 0 ; i < v.size() ; i++)
        cout << v[i].fi << " " << v[i].se << endl;
        
    return 0;
}
