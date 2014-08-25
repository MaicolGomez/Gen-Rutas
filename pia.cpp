#include<bits/stdc++.h>
using namespace std;
#define pi (2.0*acos(0.0))
#define eps 1e-6
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
#define CAP 3.0
#define LMin 5.5
#define LMax 25

struct nodo{
    int u;
    double d;
    nodo(){}
    nodo(int U,double D){ u = U , d = D; }
};

bool operator<(nodo A,nodo B){ return A.d > B.d; }

double Demanda[N][N] , Distancia[N][N];
vi adj[N];
vi cost[N];

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
            if( d[v] > d[u] + cost[u][i] ){
                d[v] = d[u] + cost[u][i];
                fa[v] = u;
                Q.push( nodo( v , d[v] ) );
            }
        }
    }
    vi route;
    int x = target;
    while( x != -1 ){
        //vis[x] = 1;
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
        for(int j = 1 ; j < r.size() ; j++)
            auxRoute.pb( r[j] );
    }
    else if( i == r.size() ){
        auxRoute = dijkstra( r[i-1] , u , n , vis );
        vi x = r;
        for(int j = 1 ; j < auxRoute.size() ; j++)
            x.pb( auxRoute[j] );
        auxRoute = x;
    }
    else{
        vis[ r[i-1] ] = 0;
        vis[ r[i] ] = 1;
        auxRoute = dijkstra( r[i-1] , u , n , vis );
        
        for(int j = 0 ; j < auxRoute.size() - 1 ; j++)
            vis[ auxRoute[j] ] = 1;
        
        vis[ r[i] ] = 0;
        vi aux2 = dijkstra( u , r[i] , n , vis );
        
        for(int j = 0 ; j < aux2.size() - 1 ; j++)
            vis[ aux2[j] ] = 0;
        
        if( auxRoute.size() == 0 or aux2.size() == 0 ) return auxRoute;
        
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
    //cout << u << " <--> " << v << endl;
    for(int i = 0 ; i < R.size() ; i++){
        bool Uroute = inRoute( R[i] , u ) , Vroute = inRoute( R[i] , v );
        if( Uroute and Vroute ) continue;
        if( Uroute )
            if( solve( R[i] , n , v , newRoute , Cost ) ) id = i;
        else if( Vroute ){
            if( solve( R[i] , n , u , newRoute , Cost ) ) id = i;
        }
        else if( solve2( R[i] , u , v , n , newRoute , Cost ) ) id = i;
    }
    /*for(int i = 0 ; i < newRoute.size() ; i++)
        cout << newRoute[i] << " - ";
    cout << endl;*/
    return mp( newRoute , Cost );
}

void update(vi &route ,int u, map<pii , double > &D){
    for(int i = 0 ; i < route.size() ; i++) if( route[i] != u )
        if( D[ mp( min( route[i] , u ) , max( route[i] , u ) ) ] != 0 ){
            D[ mp( min( route[i] , u ) , max( route[i] , u ) ) ] += CAP;
            D[ mp( min( route[i] , u ) , max( route[i] , u ) ) ] = min( 0.0 , D[ mp( min( route[i] , u ) , max( route[i] , u ) ) ] );
        }
}

vector<vi> PIA(int n,double proportionecesary){
    vector< vi > R;
    map< pii , double > D;
    map< pii , double > :: iterator it;
    double Ds = 0 , Dt = 0;
    for(int i = 0 ; i < n ; i++)
        for(int j = i + 1 ; j < n ; j++)if( i != j ){
            if( (Demanda[i][j] + Demanda[j][i]) < eps ) continue;
            D[ mp(i,j) ] = -(Demanda[i][j] + Demanda[j][i]);
            Dt += Demanda[i][j] + Demanda[j][i];
        }
    
    //for(it = D.begin() ; it != D.end() ; it++)
    //    cout << (*it).fi.fi << " " << (*it).fi.se << " " << (*it).se << endl;
    
    while( Ds < proportionecesary * Dt ){
        //for(it = D.begin() ; it != D.end() ; it++)
            //cout << (*it).fi.fi << " " << (*it).fi.se << " " << (*it).se << endl;
        vector< pair< double , pii > > V;
        for(it = D.begin() ; it != D.end() ; it++)if( abs( (*it).se ) > eps )
            V.pb( mp( (*it).se , mp( (*it).fi.fi , (*it).fi.se ) ) );
            
        sort( all(V) );
        int sz = V.size() / 3;
        int r = (random() % sz); //demand in range [0 , D.size()/3>
        while( r >= 0 and V[r].fi > -eps ) r--;
        //cout << r << endl;
        if( r == -1 ) break;
        pii q = (V[r].se);
        vi newRoute = dijkstra( q.fi , q.se , n , vector<bool> (n , 0) );
        double cost = getCost( newRoute );
        /*for(int i = 0 ; i < newRoute.size() ; i++)
            cout << newRoute[i] << " ";
        cout << endl;*/
        double costinsert = inf;
        int id = -1;
        pair< vi , double > G = Candidate( q.fi , q.se , R , n , id );
        double lastCost = inf;
        if( id != -1 ) lastCost = getCost( R[id] );
        if( id == -1 or G.se < LMin or G.se > LMax or cost < G.se - lastCost ){
            if( cost < LMin or LMax < cost ) continue;
            R.pb( newRoute );
            for(int i  = 0 ; i < newRoute.size() ; i++)
                update( newRoute , newRoute[i] , D );
        }
        else{
            if( G.se < LMin or G.se > LMax ) continue;
            if( inRoute( R[id] , q.fi ) ) update( G.fi , q.se , D );
            else if( inRoute( R[id] , q.se ) ) update( G.fi , q.fi , D );
            else update( G.fi , q.se , D ) , update( G.fi , q.fi , D );
            R[id] = G.fi;
        }
        Ds = 0;
        for(int i = 0 ; i < n ; i++)
            for(int j = i + 1 ; j < n ; j++)
                Ds += Demanda[i][j] + Demanda[j][i] + 2 * D[ mp(i,j) ];
        //cout << Dt << " " << Ds << endl;
    }
    return R;
}

int main(){
    srand( time(NULL) );
    int n , m , a , b , c;
    scanf("%d%d",&n,&m);
    for(int i = 0 ; i < m ; i++){
        scanf("%d%d%d",&a,&b,&c);
        adj[a].pb( b ); cost[a].pb( c );
        adj[b].pb( a ); cost[b].pb( c );
        Distancia[a][b] = Distancia[b][a] = c;
    }
    while( scanf("%d%d%d",&a,&b,&c) == 3 ){
        a--,b--;
        Demanda[min(a,b)][max(a,b)] = c;
    }
    vi r;
    r.pb( 1 ); r.pb( 3 ); r.pb( 4 ); r.pb( 5 );
    vector<bool> vis(n , 0);
    vis[1] = vis[3] = vis[4] = 1;
    r = insert( r.size() , r , 6 , n , vis);
    for(int i = 0 ; i < r.size() ; i++)
        cout << r[i] << " ";
    cout << endl;
    vector< vi > R = PIA( n , 0.8 );
    for(int i = 0 ; i < R.size() ; i++){
        cout << "Route #" << i + 1 << ":\n";
        for(int j = 0 ; j < R[i].size() ; j++)
            cout << R[i][j] << " ";
        cout << "\n";
    }
    return 0;
}
