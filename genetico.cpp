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
#define N 1500

double Demanda[N][N] , Distancia[N][N];
int zonasPeriferia[ N ];
int NzonasPeriferia;
int M[N][N];
int MaxCities;
int Nrutas;
bool isCentrality[ N ];
double probalityChoosePeriferia , probGetcentrality;
double LongitudMaxima;
double CROSS , MUT;
double CAP;
vi adj[N];
vector<double> cost[N];

struct Individuo{
    int genotipo[N+2];
    double aptitud;
    Individuo(){
        memset( genotipo , 0 , sizeof(genotipo) );
    }
};

struct nodo{
    int x , y , d;
    nodo(){}
    nodo(int X,int Y,int D){
        x = X , y = Y , d = D;
    }
};

bool operator<(nodo A,nodo B){ return A.d < B.d; }

void CreaRutas( vector< nodo > C ){
    sort( all(C) ); //sort pair of cities
    vector< vi > R;
    for(int ii = 0 ; ii < Nrutas ; ii++){
        vector<bool> used( N , 0 );
        int r = rand()%100;
        int curCity = C[r].x;
        double probability = 1. * random() / RAND_MAX;
        if( probability < probalityChoosePeriferia )
            curCity = zonasPeriferia[ random() % NzonasPeriferia ];
        used[ curCity ] = 1;
        vi S;
        int zonas = 1;
        double lon = 0;
        S.pb( curCity );
        while( lon <= LongitudMaxima and zonas <= MaxCities ){
            int next = 0 , ct = 0;
            vi centr;
            for(int i = 0 ; i < adj[curCity].size() ; i++){
                int vec = adj[curCity][i];
                if( used[vec] ) continue;
                if( M[curCity][vec] > ct ){
                    next = i;
                    ct = M[curCity][vec];
                }
                if( isCentrality[vec] ) centr.pb( i );
            }
            double probabilityCentr = 1. * rand() / RAND_MAX;
            int n = centr.size();
            if( n and probabilityCentr < probGetcentrality )
                next = centr[ random() % n ];
            
            lon += cost[curCity][next];
            curCity = adj[curCity][next];
            S.pb( curCity );
            zonas++;
        }
        R.pb( S );
    }
}

int makeBit(){
    if( 1 + (int) ( 10.0 * rand()/(RAND_MAX+1.0) ) > 5 )
        return 1;
    return 0;
}

double fitness(vector< pair< vi , int > > &List,Individuo X){
    vector< vi > R;
    for(int i = 0 ; i < Nrutas ; i++)
        if( X.genotipo[i] ) R.pb( List[i].fi );
    
    int numberofroutes = R.size();
    vector<int> MM[N][N];
    for(int t = 0 ; t < R.size() ; t++){
        vi &route = R[t];
        for(int i = 0 ; i < route.size() ; i++){
            int d = 0;
            for(int j = i + 1 ; j < route.size() ; j++){
                int u = route[i] , v = route[j];
                d += Distancia[u][v];
                MM[u][v].pb( d );
                MM[v][u].pb( d );
            }
        }
    }
    
    for(int i = 0 ; i < N ; i++)
        for(int j = 0 ; j < N ; j++)
            sort( all(MM[i][j]) );
    
    double C1 = 0 , C2 = 0 , Tdemanda = 0;
    for(int i = 0 ; i < N ; i++){
        for(int j = 0 ; j < N ; j++)if( i != j and Demanda[i][j] > 0 ){
            C2 += Demanda[i][j];
            Tdemanda += Demanda[i][j];
            int r = MM[i][j].size();
            double have = 0;
            for(int k = 0 ; k < MM[i][j].size() ; j++){
                if( have >= Demanda[i][j] ) break;
                C1 += min( CAP , Demanda[i][j] - have ) * MM[i][j][k];
                have += CAP;
            }
            have = min( have , Demanda[i][j] );
            C2 -= have;
        }
    }
    if(  )
    double W1 , W2 , W3;
    return W1 * C1 + W2 * C2 + W3 * numberofroutes;
}

Individuo makeIndividuo(vector< pair< vi , int > > &List,int x){
    Individuo X;
    for(int i = 0 ; i < Nrutas ; i++)
        if( List[i].se == x ) X.genotipo[i] |= 1;
    
    X.aptitud = fitness( List , X );
    return X;
}

vector<Individuo> makePoblation(vector< pair< vi , int > > &List,int Poblation){
    vector<Individuo> poblation( Poblation );
    for(int i = 0 ; i < Poblation ; i++)
        poblation[i] = makeIndividuo( List , i );
    return poblation;
}

Individuo getBest(vector<Individuo> P){
    int c = 0;
    for(int i = 0 ; i < P.size() ; i++)
        if( P[i].aptitud < P[c].aptitud ) c = i;
    
    return P[c];
}

vector<Individuo> SelectionTournoments(vector<Individuo> P){
    int n = P.size();
    vector<Individuo> NP( n );
    for(int i = 0 ; i < n - 1 ; i++){
        int a = (int) (((double) n )*rand()/(RAND_MAX+1.0));
        int b = (int) (((double) n )*rand()/(RAND_MAX+1.0));
        if( P[a].aptitud < P[b].aptitud ) NP[i] = P[a];
        else NP[i] = P[b];
    }
    NP[n - 1] = getBest( P );
    return NP;
}

vector<Individuo> crossSelection(vector< pair< vi , int > > &List ,vector<Individuo> P){
    int n = P.size();
    for(int i = 0 ; i + 1 < n ; i += 2){
        if( (double) rand()/(RAND_MAX+1.0) < CROSS ){
            int point = (int) ( ( (double) Nrutas) * rand()/( RAND_MAX + 1.0 ) );
            for(int j = point ; j < Nrutas ; j++)
                swap( P[i].genotipo[j] , P[i+1].genotipo[j] );
                
            //Mutation Process
            for(int k = i ; k < i + 2 ; k++){
                for(int j = 0 ; j < Nrutas ; j++)
                    if( (double) rand() / ( RAND_MAX + 1.0 ) < MUT )
                        P[k].genotipo[j] ^= 1;
                
                P[k].aptitud = fitness( List , P[k] );
            }
        }
    }
    return P;
}

Individuo AG( vector< pair< vi , int > > &List , int poblation ){
    random_shuffle( all(List) );
    vector<Individuo> P = makePoblation( List , poblation );
    int generations;
    for(int i = 0 ; i < generations ; i++){
        P = SelectionTournoments( P );
        P = crossSelection( List , P );
    }
    return getBest( P );
}

int main(){
    srand(time(NULL));
    //vector<nodo> C;
    //CreaRutas( C );
    vector< pair< vi , int > > List;
    Nrutas = List.size();
    int poblation;
    AG( List , poblation );
    return 0;
}
