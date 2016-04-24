#include <iostream>
#include <cassert>
#include <vector>
#include <cstdint>
#include <opencv/cv.hpp>
#include "16807.h"
#include "ndarray.h"
#include "singlelist.h"

using namespace std;
using namespace cv;
/**
 * @file percolation.h
 * @author zpj
 * @brief Some code for percolation analysis
 * @todo Data structure for graph?
 */
template<int N>
/**
 * @brief The near bond struct for N-Dimensional lattice
 *
 * + Absorb data effeciently
 * + No check for overflowing
 */
struct nbond{
    unsigned char size;
    char c[2*N];
    nbond():size(0){}
    char & operator[](int i){return c[i];}
    void append(char x){
        assert(size<2*N);
        c[size++]=x;
    }
    void clear(){size=0;}
    bool del(char ax){
        bool found=false;
        for(int i=0;i<size;i++){
            if(found){
                c[i-1]=c[i];
            }
            if(c[i]==ax){
                assert(!found);
                found=true;
            }
        }
        if(found){
            size--;
            cout<<"Found!"<<endl;
        }
        return found;
    }
};

enum visit{unvisited, inquene, popout};
inline int sign(int x){
    return (x>=0)?1:-1;
}
inline char rev(char ax, int D){
    return (ax<0)?ax+D:ax-D;
}

/**
 * @brief The combined chars union for small char arrays
 *
 * + An char array
 * + Assignment/Comparation can be parallelize.
 */
union combc{
    int8_t c[4];
    int32_t a;
    combc():a(0){}
    int8_t & operator[](int i){return c[i];}
    int32_t & operator=(const int b){return (a=b);}
    int32_t & operator=(const combc com){a=com.a; return a;}
    bool operator==(const combc com){return a==com.a;}
    bool operator!=(const combc com){return a!=com.a;}
};

template<int D>
/**
 * @brief The low dimensional torus class for D<=4
 *
 * + For low dimensional torus, the bonds can be saved by nbond
 *   whose size is only 2D+1 byte
 * + Use `combc` to save time judging wrapping
 */
class ltorus{
    ndarray<nbond<D>> bonds;
public:
    /// Wrapped clusters
    vector<int> wclus;
    ltorus(int width){
        assert(D<=4 && D>1);
        bonds=ndarray<nbond<D>>(D, width);
    }
    void setbond(double prob){
        int near;
        wclus.clear();
        if(wclus.capacity()>10){
            wclus.shrink_to_fit();
        }
        for(int curr=0;curr<bonds.size();curr++){
            bonds[curr].clear();
        }
        for(int curr=0;curr<bonds.size();curr++){
            for(int ax=0; ax<D; ax++){
                if(myrand()<prob){
                    near=bonds.rollindex(curr, ax);
                    bonds[curr].append(ax);
                    bonds[near].append(ax-D);
                }
            }
        }
    }
    void deleaf(int leaf){
        int father;
        char ax;
        do{
            ax=bonds[leaf][0];
            father=bonds.rollind(leaf, ax, D);
            bonds[leaf].clear();
            bonds[father].del(rev(ax, D));
            leaf=father;
        }while(bonds[leaf].size==1);
    }
    void debranch(){
        for(int i=0;i<bonds.size();i++){
            if(bonds[i].size==1){
                deleaf(i);
            }
        }
    }

    bool wrapping(){
        quene<int> q;                           //quene for BFS
        //status for visit, must init
        ndarray<char> status(bonds);
        status=unvisited;
        //zone for wrapping judgement. Auto init
        ndarray<combc> zone(bonds);
        //Wrapping status for Cluster/Torus
        bool cwrap=false, twrap=false;
        int delta, curr, near, ax, absax;
        for(int i=0;i<bonds.size();i++){
            if ((status[i]==unvisited) && bonds[i].size){
                zone[i]=0;
                q.append(i);
                cwrap=false;
                while(q.notempty()){
                    curr=q.pop();
                    status[curr]=popout;
                    for(int ii=0;ii<bonds[curr].size;ii++){
                        ax=bonds[curr][ii];
                        if(ax>=0){delta=1;absax=ax;}
                        else{delta=-1;absax=ax+D;}
                        near=bonds.rollindex(curr,absax,delta);
                        if(delta==sign(near-curr)) delta=0;
                        if(status[near]==unvisited){
                            zone[near]=zone[curr];//
                            zone[near][absax]+=delta;//
                            q.append(near);
                            status[near]=inquene;
                        }
                        else if(status[near]==inquene && !cwrap){
                            zone[curr][absax]+=delta;
                            if(zone[near]!=zone[curr]){
                                wclus.push_back(i);
                                cwrap=true;
                                twrap=true;
                            }
                            zone[absax][curr]-=delta;
                        }
                    }
                }
            }
        }
        return twrap;
    }
    /**
     * @brief save 2D matrix to image
     * @param filename
     * @param bdlen length of bond
     *
     * Save 2D percolation to image using openCV
     */
    template<int L>
    void savetoimg(string s){
        int width=bonds.shape(0), ax, sf=L/2, W=width*L;
        Mat M;
        assert(L>1);
        assert(D==2);
        assert(bonds.size()<1024*1024);
        ndarray<unsigned char> g(D, W);
        g=0;
        for(int i=0;i<width;i++){
            for(int j=0;j<width;j++){
                g(L*i+sf,L*j+sf)+=bonds(i,j).size*63;
                for(int k=0;k<bonds(i,j).size;k++){
                    ax=bonds(i,j)[k];
                    if(ax<0) continue;
                    for(int l=1;l<L;l++){
                        g((L*i+l*(1-ax)+sf)%W,(L*j+l*ax+sf)%W)=63;
                    }
                }
            }
        }
        M=Mat(width*L, width*L, CV_8UC1, g.head);
        imwrite(s,M);
//        if(show){
//            imshow("Test", M);
//            waitKey(0);
//        }
    }
};