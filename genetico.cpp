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

int zonasPeriferia[ N ];
int NzonasPeriferia;
int M[N][N];
int MaxCities;
int Nrutas;
bool isCentrality[ N ];
double probalityChoosePeriferia , probGetcentrality;
double LongitudMaxima;
double CROSS , MUT;
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

double fitness(Individuo X){
    
}

Individuo makeIndividuo(){
    Individuo X;
    for(int i = 0 ; i < Nrutas ; i++)
        X.genotipo[i] = makeBit();
    
    X.aptitud = fitness( X );
    return X;
}

vector<Individuo> makePoblation(int n){
    vector<Individuo> poblation( n );
    for(int i = 0 ; i < n ; i++)
        poblation[i] = makeIndividuo();
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

vector<Individuo> crossSelection(vector<Individuo> P){
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
                
                P[k].aptitud = fitness( P[k] );
            }
        }
    }
    return P;
}

Individuo AG(){
    int poblation;
    vector<Individuo> P = makePoblation( poblation );
    int generations;
    for(int i = 0 ; i < generations ; i++){
        P = SelectionTournoments( P );
        P = crossSelection( P );
    }
    return getBest( P );
}

int main(){
    srand(time(NULL));
    int Nrutas;
    vector<nodo> C;
    CreaRutas( C );
    AG();
    return 0;
}
