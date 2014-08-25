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

struct Arista{
    int VO;
    int VD;
    double distancia;
   // int direccion;
    Arista(){}
    Arista(int vo,int vd,double dist){
        VO=vo;
        VD=vd;
        distancia=dist;
    };
    bool operator<( Arista A ){ return distancia < A.distancia; }
};

struct Demanda{
    int zonaO;
    int zonaD;
    double demanda;
    double dirCub;
    double traCub;
    double demSinCubrir;
    vector<int> rutasOD;

    Demanda(){
        zonaO = 0;
        zonaD = 0;
      demanda = 0;
        dirCub = 0;
        traCub = 0;
        demSinCubrir = 0;
    };

    Demanda(int zonao, int zonad,double dem){
        zonaO = zonao;
        zonaD = zonad;
        demanda = dem;
        dirCub = 0;
        traCub = 0;
        demSinCubrir=dem;
    };
};

bool operator <(Demanda A,Demanda B){
        return A.demanda < B.demanda;
    }

struct RutaZona{
    list<int> nodosRZ;
    double distanciaTotal;
    double costoTotal;
    RutaZona(){
        costoTotal = 0;
        distanciaTotal = 0;
    };
    bool esVacio(){
        return (nodosRZ.empty() == 1);
    }
    void insertar(int v){
        nodosRZ.push_back(v);
    }
    void insertar_front(int v){
        nodosRZ.push_front(v);
    }
};

struct GrafoZona{
    int V;
    int A;
    vector< vector< Arista> > G;
    GrafoZona(){
        V = 0;
        A = 0;
    };
    GrafoZona(int v, int a){
        V = v;
        A = a;
        G.resize(V);
    };
};

vector<Demanda> construirListaDemanda(vector<vector<Demanda > > matriz){
    vector< Demanda >  arrZona ;
    int tam = matriz.size();
    for(int i = 0 ; i < tam ; i++)
        for(int j = i ; j < tam ; j++)
            if(i != j){
                double suma = matriz[i][j].demSinCubrir + matriz[j][i].demSinCubrir;
                Demanda demzona(i,j,suma);
                arrZona.push_back(demzona);
            }
    return arrZona;
}

pair<int, int> seleccionarParZona( vector< Demanda >  &arrDemanda ){
    sort( arrDemanda.begin() , arrDemanda.end() );
    int rango = ceil( (float)( arrDemanda.size() / 3 ) );
    int choose = rand()%rango;
    return mp( arrDemanda[choose].zonaO , arrDemanda[choose].zonaD );
}

pair<int, int> seleccionarParZonaIni(vector< Demanda >  &arrDemanda){
    sort( arrDemanda.begin(), arrDemanda.end() );
    return mp( arrDemanda[0].zonaO , arrDemanda[0].zonaD );
}

struct nodo{
    int vertice;
    double  distancia;
    nodo(int vert, double dist){
        vertice = vert;
        distancia = dist;
    }
};

bool operator<(nodo A,nodo B){ return A.distancia > B.distancia; }

bool esProhibido(list<int> lista, int nodo){
    for (list<int>::iterator it = lista.begin();it!= lista.end();it++)
        if( nodo == *it ) return 1;
    return 0;
}

RutaZona DijkstraZona(GrafoZona grafo,  pair <int, int> parZona, RutaZona ruta, int tipo){//falta crear la ruta
    RutaZona a;
    double distancia[grafo.V];
    bool visitado[grafo.V];
    int previo[grafo.V];
    list<int> prohibido;
    priority_queue<nodo> cola;
    list<int>::iterator it;
   // cout<<"excepcion"<<parZona.second;
    for(it=ruta.nodosRZ.begin();it!=ruta.nodosRZ.end();it++){
            if(tipo == 1 && *it!= parZona.second){
                prohibido.push_back(*it);
            }
            else if(tipo == 2 && *it!= parZona.first){
                prohibido.push_back(*it);
            }else if(tipo == 3 && *it!= parZona.first && *it!= parZona.second){
                    prohibido.push_back(*it);
            }
    }

    for(int i=0;i<grafo.V;i++){
        distancia[i] = INFINITY;
        visitado[i] = false;
        previo[i] = -1;
    }

    cola.push(nodo(parZona.first, 0));
    distancia[parZona.first]=0;

    int actual, adyacente;
    double dist;

    while(!cola.empty()){
        actual = cola.top().vertice;
        cola.pop();
        if(visitado[actual])continue;
        visitado[actual] = true;
        for(unsigned int i=0;i < grafo.G[actual].size();i++){
             if( !esProhibido(prohibido, grafo.G[actual][i].VD) ){
                adyacente = grafo.G[actual][i].VD;
                dist = grafo.G[actual][i].distancia;
                    if( distancia[ actual ] + dist < distancia[ adyacente ] ){
                        distancia[ adyacente ] = distancia[ actual ] + dist;  //relajamos el vertice actualizando la distancia
                        previo[ adyacente ] = actual;                         //a su vez actualizamos el vertice previo
                        cola.push( nodo( adyacente , distancia[adyacente] ) ); //agregamos adyacente a la cola de prioridad
                    }
            }
        }
    }
    int destino = parZona.second;
    a.distanciaTotal = distancia[destino];
    while( previo[destino] != -1 ){
        a.insertar_front(destino);
        destino = previo[destino];
    }
    if(parZona.first != parZona.second)  a.insertar_front(destino);
    return a;
}

RutaZona creaRutaDirecta( GrafoZona grafo, pair<int, int> parZona, int distMin, int distMax, RutaZona rutaOrig, int tipo){
    RutaZona ruta = DijkstraZona(grafo,parZona,rutaOrig,tipo);// 1 para indicar al dijkstra que si permitael primer vertice buscar los adyacentes al primer vertice
    if( !ruta.esVacio() && ruta.distanciaTotal<=distMax && ruta.distanciaTotal>=distMin)
        return ruta;
    else{
        RutaZona vacio;//retornamos una ruta vacia
        vacio.distanciaTotal = INFINITY;
        vacio.costoTotal = INFINITY;
        return vacio;
    }
}

bool nodoEnRuta(int nodo, RutaZona ruta){
list<int>::iterator it;
    for (it = ruta.nodosRZ.begin();it!= ruta.nodosRZ.end();it++)
        if(nodo == *it) return 1;
    return 0;
}

bool nodoEnVector(int nodo, vector<int> vecto){
    for(int i = 0 ; i < (int)vecto.size() ; i++)
        if( nodo == vecto[i] ) return 1;
    return false;
}

RutaZona insercionPrincipio(GrafoZona grafo,RutaZona ruta ,  int nodo, int distMin, int distMax, bool &huboInsercion){
    list<int>::iterator it = ruta.nodosRZ.begin();
    pair<int, int >par( nodo , (*it));
    RutaZona ruta1 ;
    huboInsercion = false;
    ruta1 = creaRutaDirecta(grafo,par,distMin,distMax, ruta,1);
    RutaZona rutaRes;
    if(!ruta1.esVacio()){
        rutaRes = ruta;
        it=ruta1.nodosRZ.end();
        it--;
        it= ruta1.nodosRZ.erase(it);
        for(it--;it!=ruta1.nodosRZ.begin();it--){
            rutaRes.insertar_front(*it);
        }
        rutaRes.insertar_front(*it);
        rutaRes.costoTotal = ruta.costoTotal + ruta1.costoTotal;
        rutaRes.distanciaTotal = ruta.distanciaTotal + ruta1.distanciaTotal;
        huboInsercion=true;
    }
    return rutaRes;
}

double localizarDist(GrafoZona grafo, int it, int it2){
    double dist = 0;
    for(int i = 0; i < grafo.G[it].size();i++)
        if(grafo.G[it][i].VD == (it2) )
            dist = grafo.G[it][i].distancia;
    return dist;
}

RutaZona insercion(GrafoZona grafo,RutaZona ruta, list<int>::iterator it, int destino, int distMin, int distMax, bool &huboInsercion /*  itCap  */){
    RutaZona rutaRes;
    huboInsercion = 0c;
    list<int>::iterator it3;
    list<int>::iterator it2 = it;
    it2++;
    pair<int, int> par1((*it), destino);
    pair<int, int> par2(destino, (*it2));
    RutaZona ruta1;
    RutaZona ruta2;
    ruta1 = creaRutaDirecta(grafo, par1, distMin, distMax, ruta, 2);
    ruta2 = creaRutaDirecta(grafo, par2, distMin, distMax, ruta, 1);
    if(!ruta1.esVacio() && !ruta2.esVacio()){
        rutaRes = ruta;
        list<int>::iterator itcopia;
        itcopia = rutaRes.nodosRZ.begin();
        for(it3 = ruta.nodosRZ.begin(); *it3!= *it;it3++)
            itcopia++;
        double dist;
        dist = localizarDist(grafo,*it,*it2);
        ruta2.nodosRZ.erase(ruta2.nodosRZ.begin());
        ruta1.nodosRZ.splice(ruta1.nodosRZ.end(), ruta2.nodosRZ);
        ruta1.nodosRZ.erase(ruta1.nodosRZ.begin());

        it3 = ruta1.nodosRZ.end();
        it3--;
        int nodoNoInsertar = *it3;
        itcopia++;
        for(it3=ruta1.nodosRZ.begin() ; it3 != ruta1.nodosRZ.end() ; it3++)
            if(*it3 != nodoNoInsertar )
                rutaRes.nodosRZ.insert(itcopia,*it3);
        rutaRes.costoTotal =ruta.costoTotal +ruta1.costoTotal + ruta2.costoTotal;
        rutaRes.distanciaTotal = ruta.distanciaTotal + ruta1.distanciaTotal + ruta2.distanciaTotal - dist;
        huboInsercion = true;
    }
    return rutaRes;
}

RutaZona insercionFinal(GrafoZona grafo,RutaZona ruta ,  int nodo, int distMin, int distMax, bool &huboInsercion){
   list<int>::iterator it = ruta.nodosRZ.end();
    it--;
    pair<int, int> par((*it), nodo);
    RutaZona ruta1 ;
    huboInsercion = false;
    RutaZona rutaRes;
    ruta1 = creaRutaDirecta(grafo, par, distMin, distMax, ruta, 2);
    if( !ruta1.esVacio() ){
        rutaRes = ruta;
        it = ruta1.nodosRZ.begin();
        ruta1.nodosRZ.erase(it);
        for(it=ruta1.nodosRZ.begin();it!=ruta1.nodosRZ.end();it++)
                rutaRes.insertar(*it);
        
        rutaRes.costoTotal = ruta.costoTotal + ruta1.costoTotal;
        rutaRes.distanciaTotal = ruta.distanciaTotal + ruta1.distanciaTotal;
        huboInsercion = true;
    }
    return rutaRes;
}


RutaZona insertarEnMejorRuta( GrafoZona grafo, pair<int, int> parZona, vector<RutaZona> rutas, RutaZona &rutaSel, int &pos, int distMin, int distMax){
    RutaZona ruta;
    RutaZona rutaAux;
    RutaZona rutaRes;//rutaresultado
    rutaRes.distanciaTotal = INFINITY;
    rutaRes.costoTotal = INFINITY;
    bool huboInsercion = 0;
    pair<int, int> par;
    list<int>::iterator it;
    list<int>::iterator fin;

    for(int i = 0 ; i < rutas.size() ; i++){
        ruta = rutas[i];
        fin=ruta.nodosRZ.end();
        fin--;
        if(nodoEnRuta(parZona.first, ruta) && nodoEnRuta(parZona.second, ruta)){
            break;
        }
        else  if(nodoEnRuta(parZona.first, ruta ) ){//si se encuentra el primer nodo
            rutaAux = insercionPrincipio(grafo,ruta, parZona.second, distMin, distMax, huboInsercion );
            if(huboInsercion && rutaAux.distanciaTotal < rutaRes.distanciaTotal){//en vez  de distancia es costoTotal
                rutaRes = rutaAux;
                rutaSel = ruta;
                pos = i;
            }
            for(it = ruta.nodosRZ.begin() ; it!= fin ; it++){
                rutaAux = insercion(grafo,ruta, it, parZona.second, distMin, distMax, huboInsercion /*, itcap*/);
                if(huboInsercion && rutaAux.distanciaTotal < rutaRes.distanciaTotal ){
                     rutaRes = rutaAux;
                     rutaSel = ruta;
                     pos = i;
                }
            }
            rutaAux = insercionFinal(grafo,ruta, parZona.second, distMin, distMax, huboInsercion);
            if(huboInsercion && rutaAux.distanciaTotal < rutaRes.distanciaTotal){
                rutaRes = rutaAux;
                rutaSel = ruta;
                pos = i;
            }
        }
        else if(nodoEnRuta(parZona.second, ruta)){//si se encuentra el segundo nodo
            rutaAux = insercionPrincipio(grafo,ruta, parZona.first, distMin, distMax, huboInsercion );
            if(huboInsercion && rutaAux.distanciaTotal < rutaRes.distanciaTotal){
                    rutaRes = rutaAux;
                    rutaSel = ruta;
                    pos = i;
            }
            for(it = ruta.nodosRZ.begin(); it!= fin;it++){
                rutaAux = insercion(grafo,ruta, it, parZona.first,distMin, distMax, huboInsercion/*, itcap*/);
                if(huboInsercion && rutaAux.distanciaTotal < rutaRes.distanciaTotal ){
                     rutaRes = rutaAux;
                     rutaSel = ruta;
                     pos = i;
                }
            }
            rutaAux = insercionFinal(grafo,ruta, parZona.first, distMin, distMax, huboInsercion);
             if(huboInsercion && rutaAux.distanciaTotal < rutaRes.distanciaTotal){
                    rutaRes = rutaAux;
                    rutaSel = ruta;
                    pos = i;
            }
        }
        else{//si no se encuentra ninguno
            RutaZona rutaPre;
            rutaPre.distanciaTotal = INFINITY;
            rutaAux = insercionPrincipio(grafo,ruta, parZona.first, distMin, distMax, huboInsercion );
            if(huboInsercion && rutaAux.distanciaTotal < rutaPre.distanciaTotal)
                    rutaPre = rutaAux;
            for(it = ruta.nodosRZ.begin(); it!= fin;it++){
                rutaAux = insercion(grafo, ruta, it, parZona.first, distMin, distMax, huboInsercion/*, itcap*/);
                if(huboInsercion && rutaAux.distanciaTotal < rutaPre.distanciaTotal)
                    rutaPre = rutaAux;                
            }
            rutaAux = insercionFinal(grafo,ruta, parZona.first, distMin, distMax, huboInsercion);
             if(huboInsercion && rutaAux.distanciaTotal < rutaPre.distanciaTotal){
                    rutaPre= rutaAux;
             }
            if(huboInsercion){
                fin=rutaPre.nodosRZ.end();
                fin--;
                rutaAux = insercionPrincipio(grafo,rutaPre, parZona.second, distMin, distMax, huboInsercion );
                if(huboInsercion && rutaAux.distanciaTotal < rutaRes.distanciaTotal){
                        rutaRes = rutaAux;
                        rutaSel = ruta;
                        pos = i;
                }
                for( it = rutaPre.nodosRZ.begin(); it!= fin; it++){
                    rutaAux= insercion(grafo,rutaPre, it,parZona.second,distMin, distMax, huboInsercion/*, itcap*/);
                    if(huboInsercion && rutaAux.distanciaTotal< rutaRes.distanciaTotal){
                        rutaRes = rutaAux;
                        rutaSel = ruta;
                        pos = i;
                    }
                }
                rutaAux = insercionFinal(grafo,rutaPre, parZona.second, distMin, distMax, huboInsercion);
                if(huboInsercion && rutaAux.distanciaTotal < rutaRes.distanciaTotal){
                    rutaRes = rutaAux;
                    rutaSel = ruta;
                    pos = i;
                }
            }
        }
    }
    return rutaRes;
}

void procesoActualizaInverso(vector<vector<Demanda> > &matriz, double cap, int itPrim, int itSecond , double &demcub, RutaZona ruta, int idRuta){
    double demandaDir = 0;
    demandaDir= matriz[(itPrim)][(itSecond)].dirCub;
    if( demandaDir > 0 && demandaDir >= cap ){
         matriz[(itPrim) ][(itSecond)].demSinCubrir+= cap;
         matriz[(itPrim) ][(itSecond)].dirCub-= cap;
        demcub=demcub-cap;
    }
    else if( demandaDir > 0 && demandaDir < cap ){
        matriz[(itPrim) ][(itSecond)].demSinCubrir += demandaDir;
        matriz[(itPrim) ][(itSecond)].dirCub = 0;
        demcub=demcub-demandaDir;
    }
    vector<int>::iterator it;
    for(it = matriz[(itPrim)][(itSecond)].rutasOD.begin() ; it!=matriz[(itPrim)][(itSecond)].rutasOD.end() ; it++)
        if(*it== idRuta){
            matriz[(itPrim)][(itSecond)].rutasOD.erase(it);
            break;
        }
}

void procesoActualiza(vector<vector<Demanda> > &matriz, double cap, int itPrim, int itSecond , double &demcub, RutaZona ruta, int idRuta){
     double demanda = 0;
     demanda =  matriz[(itPrim)][(itSecond)].demSinCubrir;
     if(demanda > 0 && demanda >= cap){
         matriz[(itPrim) ][(itSecond)].demSinCubrir-= cap;
         matriz[(itPrim) ][(itSecond)].dirCub+= cap;
         demcub = demcub + cap;
     }
     else if(demanda > 0 && demanda < cap){
        matriz[(itPrim) ][(itSecond)].demSinCubrir = 0;
        matriz[(itPrim) ][(itSecond)].dirCub  += demanda;
        demcub=demcub+demanda;
    }
    if(!nodoEnVector(idRuta,matriz[itPrim][itSecond].rutasOD ))matriz[itPrim][itSecond].rutasOD.push_back(idRuta);
}

void actualiza(vector<vector<Demanda> > &matriz,list<int>::iterator it,list<int>::iterator it2, RutaZona ruta , double &demcub, int idRuta,double X){
    list<int>::iterator itRec;
    double cap = X; //AQUI SERA EL X DE MAYCOL
    procesoActualiza(matriz, cap, *it2, *it, demcub ,ruta,idRuta);
    procesoActualiza(matriz, cap, *it, *it2,demcub, ruta,idRuta);
}

void actualizarMatrizOD(vector<vector<Demanda> > &matriz , RutaZona ruta,double &demcub, int idruta,double X){//faltan datos
    list<int>::iterator it;
    list<int>::iterator it2;
    list<int>::iterator itRec;
    for(it = ruta.nodosRZ.begin() ; it!= ruta.nodosRZ.end() ; it++)
        for(it2 = it; it2!= ruta.nodosRZ.end();it2++)
           actualiza(matriz , it , it2 , ruta , demcub , idruta , X);
}

void actualizaPorInsercion(vector<vector<Demanda> > &matriz, pair<int, int> par, RutaZona ruta , double &demcub, int idRuta, double cap){
    list<int>::iterator it;
    for(it=ruta.nodosRZ.begin();it!= ruta.nodosRZ.end();it++)
       if(*it!=par.first){
            procesoActualiza(matriz,cap,*it,par.first, demcub, ruta, idRuta);
            procesoActualiza(matriz,cap,par.first,*it, demcub, ruta, idRuta);
        }
    for(it=ruta.nodosRZ.begin();it!= ruta.nodosRZ.end();it++){
       if(*it!=par.second && *it!=par.first)
            procesoActualiza(matriz,cap,*it,par.second, demcub, ruta, idRuta);
            procesoActualiza(matriz,cap,par.second,*it, demcub, ruta, idRuta);
        }
}

vector<RutaZona> getRutas(GrafoZona grafo, vector< vector<Demanda> > &matriz, double demMin, int distMin, int distMax, double demTotal, double demCub,double X){
    vector<RutaZona> vectorRutas;
    vector<Demanda>  arrDemanda;
    while( demCub <= demMin * demTotal ){
        arrDemanda = construirListaDemanda( matriz );
        pair<int, int> parZona = seleccionarParZonaIni( arrDemanda );
       // cout<<"a:"<<parZona.first<<"b:"<<parZona.second<<endl;
        RutaZona rutaDir;
        RutaZona rutVacio;

        rutaDir = creaRutaDirecta( grafo , parZona , distMin , distMax , rutVacio , 1 );
       // imprimirRuta(rutaDir);
       // cout<<"tamdirdrecta:"<<rutaDir.nodosRZ.size()<<endl;
        RutaZona rutaSel;
        int posicionRuta;
        RutaZona rutaIns;
        rutaIns = insertarEnMejorRuta(grafo, parZona, vectorRutas, rutaSel, posicionRuta , distMin, distMax);//CREO Q NO NECESITO RUTASEL
        //imprimirRuta(rutaIns);
      //  cout<<" tamin insert:"<<rutaIns.nodosRZ.size()<<"dist"<<rutaIns.distanciaTotal<<endl;
            if( rutaDir.esVacio() && rutaIns.esVacio() )  continue;
            if( rutaDir.distanciaTotal < rutaIns.distanciaTotal - rutaSel.distanciaTotal ){//en ves de distancia es costo
                vectorRutas.push_back(rutaDir);
                actualizarMatrizOD( matriz, rutaDir , demCub , vectorRutas.size() - 1 , X );//pasamos la ultima posicion porq es una nueva ruta
                 //imprimirRuta(rutaDir);
            }
            else{
                //vectorRutas[posicionRuta].clear()
                double cap = X;
                actualizaPorInsercion(matriz, parZona,rutaIns, demCub, posicionRuta, cap);
                vectorRutas[posicionRuta] = rutaIns;
                //imprimirRuta(rutaIns);
            }
    }
    return vectorRutas;
}

int main(){
    
    return 0;
}
