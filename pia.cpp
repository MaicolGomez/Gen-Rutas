#include<bits/stdc++.h>
using namespace std;
#define pi (2.0*acos(0.0))
#define eps 1e-5
#define ll long long
#define inf (1LL<<50)
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
#define N 500

#define LMin 0
#define LMax (1LL<<60)

struct nodo{
    int u;
    double d;
    nodo(){}
    nodo(int U,double D){ u = U , d = D; }
};

bool operator<(nodo A,nodo B){ return A.d > B.d; }

double Demanda[N][N] , Distancia[N][N];
vi adj[N];
vector<double> cost[N];
double CAP;

vi dijkstra(int source,int target,int n,vector<bool> vis){
    priority_queue< nodo > Q;
    Q.push( nodo( source , 0 ) );
    vi fa( n , -1 );
    vector<double> d( n , inf );
    d[source] = 0;
    while( !Q.empty() ){
        nodo q = Q.top(); Q.pop();
        int u = q.u;
        double distance = q.d;
        if( u == target ) break; //get target
        if( vis[u] ) continue;
        vis[u] = 1;
        for(int i = adj[u].size() - 1 ; i >= 0 ; i--){
            int v = adj[u][i];
            if( vis[v] ) continue;
            if( d[v] > d[u] + cost[u][i] ){
                d[v] = d[u] + cost[u][i];
                fa[v] = u;
                Q.push( nodo( v , d[v] ) );
            }
        }
    }
    vi route;
    if( d[target] > inf - 1 ) return route;
    int x = target;
    while( x != -1 ){
        route.pb( x );
        x = fa[x];
    }
    reverse( all(route) );
    return route;
}

bool inRoute(vi &route,int u){
    for(int i = 0 ; i < route.size() ; i++)
        if( route[i] == u ) return 1;
    return 0;
}

double getCost(vi &route){
    if( route.size() == 0 ) return inf;
    double cost = 0;
    for(int i = 0 ; i < route.size() - 1 ; i++)
        cost += Distancia[ route[i] ][ route[i+1] ];
    return cost;
}

vi insert(int i,vi &r,int u,int n,vector<bool> vis){
    vi auxRoute;
    if( i == 0 ){
        auxRoute = dijkstra( u , r[i] , n , vis );
        if( auxRoute.size() == 0 ) return auxRoute;
        for(int j = 1 ; j < r.size() ; j++)
            auxRoute.pb( r[j] );
    }
    else if( i == r.size() ){
        auxRoute = dijkstra( r[i-1] , u , n , vis );
        if( auxRoute.size() == 0 ) return auxRoute;
        vi x = r;
        for(int j = 1 ; j < auxRoute.size() ; j++)
            x.pb( auxRoute[j] );
        auxRoute = x;
    }
    else{
        vis[ r[i-1] ] = 0;
        vis[ r[i] ] = 1;
        auxRoute = dijkstra( r[i-1] , u , n , vis );
        
        if( auxRoute.size() == 0 ) return auxRoute;
        
        for(int j = 0 ; j < auxRoute.size() - 1 ; j++)
            vis[ auxRoute[j] ] = 1;
        
        vis[ r[i] ] = 0;
        vi aux2 = dijkstra( u , r[i] , n , vis );
        if( aux2.size() == 0 ) return aux2;
        
        for(int j = 0 ; j < aux2.size() - 1 ; j++)
            vis[ aux2[j] ] = 0;
        
        for(int j = 1 ; j < aux2.size() ; j++)
            auxRoute.pb( aux2[j] );
        
        vi x;
        for(int j = 0 ; j < r.size() ; j++){
            x.pb( r[j] );
            if( r[j] == r[i-1] ){
                for(int k = 1 ; k < auxRoute.size() - 1 ; k++)
                    x.pb( auxRoute[k] );
            }
        }
        auxRoute = x;
    }
    return auxRoute;
}

bool solve(vi &r,int n,int u,vi &newRoute,double &Cost){
    bool in = 0;
    vector<bool> vis( n , 0 );
    for(int i = 0 ; i < r.size() ; i++)
        vis[ r[i] ] = 1;
    
    for(int i = 0 ; i <= r.size() ; i++){
        if( i < r.size() ) vis[ r[i] ] = 0;
        else vis[ r[i - 1] ] = 0;
        vi auxRoute = insert( i , r , u , n , vis );
        if( i < r.size() ) vis[ r[i] ] = 1;
        else vis[ r[i - 1] ] = 0;
        if( auxRoute.size() == 0 ) continue;
        double auxCost = getCost( auxRoute );
        if( auxCost < Cost ){
            in = 1;
            Cost = auxCost;
            newRoute = auxRoute;
        }
    }
    return in;
}

bool solve2(vi &r,int u,int v,int n,vi &newRoute,double &Cost){
    bool in = 0;
    vector<bool> vis( n , 0 );
    for(int i = 0 ; i < r.size() ; i++)
        vis[ r[i] ] = 1;
    
    for(int i = 0 ; i < r.size() ; i++){
        vi aux = insert( i , r , u , n , vis );
        if( aux.size() <= r.size() ) continue;
        vector<bool> newvis = vis;
        for(int j = 0 ; j < aux.size() ; j++)
            vis[ aux[j] ] = 1;
            
        for(int j = 0 ; j < aux.size() ; j++)if( i != j ){
            vi aux2 = insert( j , aux , v , n , newvis );
            double auxCost = getCost( aux2 );
            if( auxCost < Cost ){
                in = 1;
                Cost = auxCost;
                newRoute = aux2;
            }
        }
    }
    return in;
}

pair< vi , double> Candidate(int u,int v,vector<vi> &R,int n,int &id){
    if( R.size() == 0 ) return mp( vi (1,0) , inf );
    vi newRoute;
    double Cost = inf;
    for(int i = 0 ; i < R.size() ; i++){
        bool Uroute = inRoute( R[i] , u ) , Vroute = inRoute( R[i] , v );
        if( Uroute and Vroute ) continue;
        if( Uroute ){
            if( solve( R[i] , n , v , newRoute , Cost ) ) id = i;
        }
        else if( Vroute ){
            if( solve( R[i] , n , u , newRoute , Cost ) ) id = i;
        }
        else if( solve2( R[i] , u , v , n , newRoute , Cost ) ) id = i;
    }
    return mp( newRoute , Cost );
}

void update(vi &route ,int u,double &Ds){
    for(int i = 0 ; i < route.size() ; i++) if( route[i] != u ){
        int v = route[i];
        double r = min( CAP , Demanda[u][v] );
        Demanda[u][v] -= r;
        Ds += r;
    }
}

inline bool compare(pair< double , pii > A , pair< double , pii > B){
    if( abs( A.fi - B.fi ) > eps ) return A.fi > B.fi;
    if( A.se.fi != B.se.fi ) return A.se.fi < B.se.fi;
    return A.se.se < B.se.se;
}

vector<vi> PIA(int n,double proportionnecesary){
    vector< vi > R;
    vector< pii > need;
    double Ds = 0 , Dt = 0;
    for(int i = 0 ; i < n ; i++)
        for(int j = 0 ; j < n ; j++)if( i != j ){
            if( Demanda[i][j] < eps ) continue;
            Dt += Demanda[i][j];
            if( i < j ) need.pb( mp( i , j ) );
        }
    double DLast = 0;
    while( Ds < proportionnecesary * Dt ){  
        vector< pair< double , pii > > V;
        for(int i = 0 ; i < need.size() ; i++){
            int a = need[i].fi , b = need[i].se;
            if( a < b and Demanda[a][b] + Demanda[b][a] > eps ) V.pb( mp( (Demanda[a][b] + Demanda[b][a]) , mp( a , b ) ) );
        }
        
        sort( all(V) , compare );
        int sz = V.size() / 3;
        int r = (sz>0)?(random() % sz):0; //demand in range [0 , D.size()/3> DEBUG
        pii q = V[ r ].se;
        vi newRoute = dijkstra( q.fi , q.se , n , vector<bool> (n , 0) );
        
        double cost = getCost( newRoute );
        double costinsert = inf;
        int id = -1;
        pair< vi , double > G = Candidate( q.fi , q.se , R , n , id );
        double lastCost = inf;
        if( id != -1 ) lastCost = getCost( R[id] );
        if( id == -1 or G.se < LMin or G.se > LMax or cost < G.se - lastCost ){
            if( cost < LMin or LMax < cost ) continue;
            R.pb( newRoute );
            /*for(int i  = 0 ; i < newRoute.size() ; i++)
                cout << newRoute[i] << " ";
            cout << endl;*/
            for(int i  = 0 ; i < newRoute.size() ; i++)
                update( newRoute , newRoute[i] , Ds );
        }
        else{
            if( G.se < LMin or G.se > LMax ) continue;
            int it1 = 0 , it2 = 0;
            while( it2 < G.fi.size() ){
                if( it1 < R[id].size() )
                    if( R[id][it1++] == G.fi[it2] ){
                        it2++;
                        continue;
                    }
                update( G.fi , G.fi[it2] , Ds );
                it2++;
            }
            R[id] = G.fi;
        }
        printf("%d: %lf %lf\n", (int)R.size() , Dt , Ds );
        if( Ds - DLast < eps ) break;
        DLast = Ds;
    }
    return R;
}

int main(){
    srand( time(NULL) );
    int Buses = 40 , capacity = 80 , token;
    CAP = (Buses * capacity)/10;
    int n , m , a , b;
    double c;
    token = scanf("%d%d",&n,&m);
    for(int i = 0 ; i < m ; i++){
        token = scanf("%d%d%lf",&a,&b,&c);
        adj[a].pb( b ); cost[a].pb( c );
        adj[b].pb( a ); cost[b].pb( c );
        Distancia[a][b] = Distancia[b][a] = c;
    }
    int tt;
    token = sc(tt);
    while( tt-- ){
        token = scanf("%d%d%lf",&a,&b,&c);
        Demanda[a][b] = c;
    }
    int Poblation;
    token = sc(Poblation);
    //vector< pair< vi , int > > List;
    for(int i = 0 ; i < Poblation ; i++){
        vector< vi > R = PIA(n , 0.8);
        cout << i + 1 << ":\n";
        for(int j = 0 ; j < R.size() ; j++){
            for(int k = 0 ; k < R[j].size() ; k++)
                printf("%d ",R[j][k]);
            printf("\n");
        }
    }
    return 0;
}
