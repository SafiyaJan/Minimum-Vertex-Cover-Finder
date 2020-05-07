#include <iostream>
#include <list>
#include <vector>
#include <regex>
#include <iterator>
#include <algorithm> 
#include <string>
#include <stack>
#include <list>
#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <pthread.h>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

// used to calculate approx. ratio (Remember: CNF is denominator of approx. ratio)
double cnf_size = 0;
double approx_1_size = 0;
double approx_2_size = 0;


string cnf_sat_res = "CNF-SAT-VC: timeout\n";
string approx_vc1 = "";
string approx_vc2 = "" ;

double cnf_sat_runtime = 0.0;
double approx_vc1_runtime = 0.0;
double approx_vc2_runtime = 0.0;


// struct to store arguments for algorithm functions
struct thread_args{

    unsigned int V;
    std::vector<unsigned int> *adj_list;

};

/********* Helper Functions *********/

void printGraph(vector<unsigned int> adj_list[], unsigned int V) { 
    for (unsigned int i = 0; i < V; i++) { 
        cout << i << ": ";

        for (unsigned int j = 0; j < adj_list[i].size(); j++) {

           cout << adj_list[i][j] << " ";
        }

        cout << endl;
    } 
} 


void add_edge(vector<unsigned int> adj_list[], unsigned int u, unsigned int v){ //creates an adjacency list (ie. array of arrays)
    
    adj_list[u].push_back(v); //add v into list u
    adj_list[v].push_back(u); //add u into list v

}


void remove_edge_vc1(vector<unsigned int> adj_list[], unsigned int u, unsigned int v) {
   
   // remove edge from u to v
   vector<unsigned int>::iterator it  = std::find (adj_list[u].begin(), adj_list[u].end(), v);
   adj_list[u].erase(it);

   // remove edge from v to u
   it  = std::find(adj_list[v].begin(), adj_list[v].end(), u);
   adj_list[v].erase(it);

}


// removes all edges connected to vertex
std::vector<unsigned int>* remove_edge_vc2(unsigned vertex, std::vector<unsigned int> adj_list[]){

    
    // go through the edge list for the current vertex, 
    // remove curent vertex from each of the edge lists of the verties in the current vertex edge list 
    for (auto j = adj_list[vertex].begin(); j != adj_list[vertex].end(); j++){

        adj_list[*j].erase(std::remove(adj_list[*j].begin(), adj_list[*j].end(), vertex), adj_list[*j].end());

    }

    // remove all the vertices from te current vertex edge list by clearing 
    adj_list[vertex].clear();

    // return the altered edge list
    return adj_list;

}

/********* End of Helper Functions *********/




/********* Algorithm Implementations *********/

// void approx_vc_1(unsigned int V, vector<unsigned int> adj_list[]){
void* approx_vc_1(void* args){


    //struct timespec ts;
    //clockid_t cid;

    //start timer
    //pthread_getcpuclockid(pthread_self(), &cid);

    struct thread_args *args_list = (struct thread_args *)args;
    
    std::vector<unsigned int>* adj_list = args_list->adj_list;
    unsigned int V = args_list->V;

    //APPROX-VC-1 PROCESS:
        // Step 1) Find vertex with maximum number of incident edges
        // Step 2) Add vertex to vertex cover
        // Step 3) Delete all edges incident to that vertex (from both vertice entries!)
        // Step 4) Repeat till no edges remain
    std::vector<unsigned int> vc;
    unsigned int count = 0;
    bool done = false;

    // printGraph(adj_list,V);
    // std::cout << std::endl;

    //Repeats this process until no vertices remain in edge list 
    while(count != V){

        for (unsigned int x = 0; x < V; x++){
            // cout << adj_list[x].size() << endl;
            if (adj_list[x].size() == 0){
                count += 1;
            }

            if (count == V)
                done = true;
        }

        

        if (done == true)
            break;

        unsigned int incident_edge_location = 0; //Denotes location of highest degree vertice in list
        unsigned int incident_edge_size = 0; //Denotes size of highest degree vertice in list

        //Traverses entire adjacency list to find vertex of highest degree 
        for (unsigned int i = 0; i < V; i++){
            if (adj_list[i].size() > incident_edge_size){

                incident_edge_location = i;
                incident_edge_size = adj_list[i].size();
            } 
        }
        //Step 1 Completed --> At this point, vertice with highest degree is found!  

        // vertex_cover.append(to_string(incident_edge_location) + ","); 
        vc.push_back(incident_edge_location);

        //Step 2 Completed --> Highest degree vertex added to vertex cover 


        //Deleting all edges incident to highest degree vertex
        int size = adj_list[incident_edge_location].size();

        for (int j = 0; j < size; j++){ //The size changes everytime you remove an element! Must make sure loop conditions are constant
            int value = adj_list[incident_edge_location][0]; //After deleting, elements get pushed back into "value" index --> no need to use "j" for this!

            remove_edge_vc1(adj_list,incident_edge_location,value);
        }
        //Step 3 completed --> all edges incident to highest degree vertex are deleted!
        //The program shall now repeat until adjacency list is empty!    

        count = 0;    
    }

    // printGraph(adj_list,V);

    //Step 4 completed --> adjacency list is now empty! Vertex over is outputted:

     //sort vector 
    sort(vc.begin(),vc.end());
    approx_1_size = vc.size();

    //print our vertex cover
    string result = "APPROX-VC-1: ";
    unsigned int i = 0;
    
    for(i = 0; i < vc.size()-1; i++)
        result = result + to_string(vc[i]) + ",";
           
    result = result + to_string(vc[i]) + "\n";

    approx_vc1 = result;

    //stop timer
    //clock_gettime(cid, &ts);

    //saves the runtime in microsecs
    //approx_vc1_runtime = (((double)ts.tv_nsec)*0.001);

    vc.clear();

    // free memory allocated for arguments
    free(args);

    pthread_exit(NULL);
}



void* approx_vc_2(void* args){

    
    //struct timespec ts;
    //clockid_t cid;

    //start timer
    //pthread_getcpuclockid(pthread_self(), &cid);

    struct thread_args *args_list = (struct thread_args *)args;

    std::vector<unsigned int>* adj_list = args_list->adj_list;
    unsigned int V = args_list->V;

    
    /* Steps for Algo;
       1 - Pick an edge (i.j) and remove all edges attahed to i and j
       2 - Keep picking edges until all edges have been visitin/accounter for */

    // vector to store vertices in the vertex cover
    std::vector<unsigned int> vc;

    for (unsigned int i = 0; i < V; i++){

            for (auto j = adj_list[i].begin(); j != adj_list[i].end(); j++){

                //picking edge i and j and removing all edges connected to i and j
                vc.push_back(i);
                vc.push_back(*j);
                adj_list = remove_edge_vc2(i,adj_list);
                adj_list = remove_edge_vc2(*j,adj_list);
                
                break;


            }

    }

    //sort vector 
    sort(vc.begin(),vc.end());
    approx_2_size = vc.size();

    //print our vertex cover
    string result = "APPROX-VC-2: ";
    unsigned int i = 0;
    
    for(i = 0; i < vc.size()-1; i++)
        result = result + to_string(vc[i]) + ",";
           
    result = result + to_string(vc[i]) + "\n";

    // save the result 
    approx_vc2 = result;

    // vc.clear();

    //stop timer
    //clock_gettime(cid, &ts);

    //saves the runtime in microsecs
    //approx_vc2_runtime = (((double)ts.tv_nsec)*0.001);

    vc.clear();

    // free allocated memory for arguments
    free(args);

    pthread_exit(NULL);

}



void* minisat_reduction(void* args){

    //struct timespec ts;
    //clockid_t cid;

    //start timer
    //pthread_getcpuclockid(pthread_self(), &cid);

    struct thread_args *args_list = (struct thread_args *)args;

    std::vector<unsigned int>* adj_list = args_list->adj_list;
    unsigned int V = args_list->V;

    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    bool res = false; //initializing solver to be false
    unsigned int k = 1; //Initialize minimum vertex cover as one

    std::vector<unsigned int> vc;

    while(k < V){

        Minisat::Lit matrix[V][k];

        for (unsigned int x = 0; x < V; x++)
            for (unsigned int y = 0; y < k; y++){
                Minisat::Lit literal;
                literal = Minisat::mkLit(solver->newVar());
                matrix[x][y] = literal; 
            }
        
        //case 1
        for (unsigned int i = 0; i < k; i++){ //for all i which exists from k=1 to k 
            //minisat only accepts 4 arguments max for each addClause! If multiple literals exist, use one vector of literals
            Minisat::vec<Minisat::Lit> clause_1;
            
            for (unsigned int j = 0; j < V; j++){
                clause_1.push(matrix[j][i]);
            }

            solver->addClause(clause_1);
        }
            
        //case 2
        for (unsigned int m = 0; m < V; m++){
            for (unsigned int p = 0; p < k; p++){
                for (unsigned int q = 0; q < k; q++){
                    if (p<q){
                        solver->addClause(~matrix[m][p],~matrix[m][q]);
                    }
                }
            }
        }

        
        //case 3
        for (unsigned int m = 0; m < k; m++){
            for (unsigned int p = 0; p < V; p++){
                for (unsigned int q = 0; q < V; q++){
                    if (p<q){
                        solver->addClause(~matrix[p][m],~matrix[q][m]);
                    }
                }
            }
        }
    
        //case 4
        for (unsigned int i = 0; i < V; i++){
            for (auto m: adj_list[i]){
                
                if (m < i)
                    continue;

                Minisat::vec<Minisat::Lit> clause_2;
                                
                for (unsigned int x = 0; x < k; x++){
                    clause_2.push(matrix[i][x]);
                    clause_2.push(matrix[m][x]);
                }

                solver->addClause(clause_2);
            }
        }

        //Use SAT solver for the current minimum vertex cover value, k
        res = solver->solve();

        if (res == true){
        

            for (unsigned int x = 0; x < V; x++){
                
                for (unsigned int y = 0; y < k; y++){
                   
                    bool lit_val = Minisat::toInt(solver->modelValue(matrix[x][y]));
                    
                    if(lit_val == 0){
                        vc.push_back(x);
                    }
                }
            }

            std::sort(vc.begin(),vc.end());
            cnf_size = vc.size();

            //print our vertex cover
            string result = "CNF-SAT-VC: ";
            unsigned int i = 0;
            
            for(i = 0; i < vc.size()-1; i++)
                result = result + to_string(vc[i]) + ",";
                   
            result = result + to_string(vc[i]) + "\n";

            cnf_sat_res = result;

            vc.clear();

            //stop timer
            //clock_gettime(cid, &ts);

            //saves the runtime in microsecs
            //cnf_sat_runtime = (((double)ts.tv_nsec)*0.001);

            break;

        }

        else{
            solver.reset(new Minisat::Solver());
            k++;
        }
    }

    // free memory allocated for arguments
    free(args);

    pthread_exit(NULL);

}

/********* End of Algorithm Implementations *********/




// 
void threaded_vc(unsigned int num_vert, std::vector<unsigned int> adj_list_1[], vector<unsigned int> adj_list_2[], vector<unsigned int> adj_list_3[]){

    

    // creating structs to store all arguments for algo functions
    struct thread_args *args_cnf_sat = (struct thread_args*) malloc(sizeof(*args_cnf_sat));
    struct thread_args *args_vc1 = (struct thread_args*) malloc(sizeof(*args_vc1));
    struct thread_args *args_vc2 = (struct thread_args*) malloc(sizeof(*args_vc2));



    // setting values of the struct attributes with arguments
    
    if(args_cnf_sat!=NULL){
        args_cnf_sat->adj_list = adj_list_1;
        args_cnf_sat->V = num_vert;
    }


    if(args_vc1!=NULL){
        args_vc1->adj_list = adj_list_2;
        args_vc1->V = num_vert;
    }

    if(args_vc2!=NULL){
        args_vc2->adj_list = adj_list_3;
        args_vc2->V = num_vert;
    }


    // creating thread to run the algo
    // for now just creating one thread to make sure it runs
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    // run the threads for computing the Vertex Cover using the different algorithms
    pthread_create(&thread1, NULL, minisat_reduction, args_cnf_sat);
    pthread_create(&thread2, NULL, approx_vc_1, args_vc1);
    pthread_create(&thread3, NULL, approx_vc_2, args_vc2);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 120;

    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_timedjoin_np(thread1, NULL, &ts);

    //display result
    std::cout << cnf_sat_res << std::flush;
    std::cout << approx_vc1 << std::flush;
    std::cout << approx_vc2 << std::flush;

    return;

}


//main function
int main(int argc, char* argv[]){

    //local variables declared here
   smatch m;
   string input;
   unsigned int a,b,c,V = 1;
   vector <unsigned int> *adj_list_1 = NULL; //initializing adjacency list
   vector <unsigned int> *adj_list_2 = NULL;
   vector <unsigned int> *adj_list_3 = NULL;

    //matches command input to appropriate loop
   regex vert_rx("^[V]"); 
   regex edge_rx("^[E]");

   //extract relevant info from input
   const regex numbers("([0-9]+)+");
   const regex coordinates("([0-9]+[,][0-9]+)+");



   while (!cin.eof()){ //eof allows program to exit gracefully 
        
        bool flag_1 = false;
        bool flag_2 = false;
        bool flag_3 = false;
        bool flag_4 = false;



        getline(cin, input);

        if (cin.eof())
            break;

        if (regex_search(input,m,vert_rx)){
            regex_search(input, m, numbers);

            string temp = m[0];
            a = stoi(temp);

            if (a == 0){ //Error case for if vertice = 0
                cout << "Error: Graph cannot have zero vertices!" << endl;
                continue;
            }

            else
                V = a;
        }

        getline(cin, input);

        if (regex_search(input,m,edge_rx)){
            adj_list_1 = new vector<unsigned int>[V];
            adj_list_2 = new vector<unsigned int>[V];
            adj_list_3 = new vector<unsigned int>[V];


            while (regex_search(input,m,coordinates)){
                string s = m.str(0);
                
                string delimiter_1 = ",";
                string delimiter_2 = ">";
                string token_1 = s.substr(0, s.find(delimiter_1));
                string token_2 = s.substr(s.find(",")+1, s.find(delimiter_2));
                b = stoi(token_1);
                c = stoi(token_2);
                
                if (b == c){ //Error case when an edge spec includes the same vertice twice
                    cout << "Error: Cannot have an edge between a vertice and itself!" << endl;
                    flag_3 = true;
                    break;
                }

                

                if (b >= V || c >= V){ //Error case when an edge spec includes a vertex higher than total number of vertices
                    cout << "Error: Cannot have an edge between non-existant vertices." << endl;
                    flag_4 = true;
                    break;
                }


                for (unsigned int i = 0; i < adj_list_1[b].size(); i++){ //Error case for duplicate edges
                    if (adj_list_1[b][i] == c){
                        cout << "Error: System does not allow duplicate edges." << endl;
                        flag_1 = true;
                        break;
                    }
                }

                if (flag_1 == true)
                    break;

                for (unsigned int i = 0; i < adj_list_1[c].size(); i++){ //Error case for duplicate edges
                    if (adj_list_1[c][i] == b){
                        cout << "Error: System does not allow duplicate edges." << endl;
                        flag_2 = true;
                        break;
                    }
                }

                if (flag_2 == true)
                    break;
                
                if (flag_1 == false && flag_2 == false){ //If the edge spec is valid, add it to the adjacency list
                    add_edge(adj_list_1, b ,c);
                    add_edge(adj_list_2, b, c);
                    add_edge(adj_list_3, b, c);

                }

                input = m.suffix(); //Used to iterate through remaining edges
            }

        }

        if (flag_1 || flag_2 || flag_3 || flag_4 ){
            continue;
        }

        if (adj_list_1->size() == 0){

                std::cout << "CNF-SAT-VC: " << std::endl;
                std::cout << "APPROX-VC-1: " << std::endl;
                std::cout << "APPROX-VC-2: " << std::endl;
            }

        else{

            
            cnf_sat_res = "CNF-SAT-VC: timeout\n";
            approx_vc1 = "";
            string approx_vc2 = "" ;

            cnf_sat_runtime = 0.0;
            approx_vc1_runtime = 0.0;
            approx_vc2_runtime = 0.0;

            cnf_size = 0;
            approx_1_size = 0;
            approx_2_size = 0;

            // this function call, handles the whole threading functionality
            threaded_vc(V,adj_list_1,adj_list_2,adj_list_3);
            
        }

        
    }

   return 0;
} 
