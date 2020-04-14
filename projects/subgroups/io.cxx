#include "../eigen-git-mirror/Eigen/Dense"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "../eigen-git-mirror/Eigen/src/Core/util/ForwardDeclarations.h"
//#include "symop.cpp" //comment out to compile factor group
#include "crystal.hpp"//comment out to compile factor group 
//#include "categorize_symop_main.cpp"
#include "symmetry.hpp"
#include "symgroup.hpp"

using namespace std;

auto read_lattice(string filename)
{
//read file and take each word and use it to populate L, an Eigen matrix
     Eigen::Matrix3d  L;
     fstream file;
     std::string word;
     file.open(filename.c_str());
     int count=0;
     int i=0;
     while(file >> word) {
           int j=count%3;
    	   L(i,j)=std::stod(word);
	       count++;
           if(count%3==0){i++;}
           
     }
     return L;
}

CrystalStructure read_poscar(std::string filename)
{
    /*input is the path to a Poscar file
     * This function reads the file and puts the information of the lattice and basis
     * into a Crystal_structure object which is returned
     */

     Eigen::Matrix3d  lattice;
     std::vector<Eigen::Vector3d> basis;
     CrystalStructure xtal_struct(lattice, basis);
     std::ifstream file(filename);
     std::string line;
//     file.open(filename.c_str());
     std::string title;
//     std::getline(file, title);
     int ct=0;
     double val1;
     double val2;
     double val3;
     while (std::getline(file, line)){
             std::stringstream linestream(line);
 //            std::cout<<ct<<std::endl;
             if (ct==0){ct++;
                 title=line;}
             else if (ct==1){ct++; continue;}
             else if (ct>1 && ct<=4){
             linestream>>val1>>val2>>val3;
             xtal_struct.lattice(0,ct-2)=val1;
             xtal_struct.lattice(1,ct-2)=val2;
             xtal_struct.lattice(2,ct-2)=val3;
             ct++;
             }
             else if(ct>4 && ct<8){ct++;}
             else{
             linestream>>val1>>val2>>val3;
             Eigen::Vector3d temp;
             temp(0)=val1;
             temp(1)=val2;
             temp(2)=val3;
             xtal_struct.basis.push_back(temp);
             ct++;
             }
     }
     return xtal_struct;

}
void print_sub_groups(SymGroup<SymOp, BinaryComparator_f> input_group, std::vector<SymGroup<SymOp, BinaryComparator_f>> list_of_subgroups)
{//Pringing out the total group and list of subgroups.
    std::cout << "The total group has " << input_group.operations().size() << " operations: " << std::endl;
    for (SymOp operation : input_group.operations()) 
    {
        std::cout << operation.cart_matrix << std::endl << std::endl;
    }
    std::cout << "__________________________________________________________________" <<std::endl;
    std::cout << std::endl;

    std::cout << "There are " << list_of_subgroups.size() << " subgroups in the total group: " << std::endl << std::endl;
    
    int subgroup_num = 1;
    for (SymGroup<SymOp, BinaryComparator_f> subgroup : list_of_subgroups) 
    {
        std::cout << "Subgroup " << subgroup_num << " has " << subgroup.operations().size() << " operations: " << std::endl << std::endl;
        for (SymOp operation : subgroup.operations()) 
        {
            std::cout << operation.cart_matrix << std::endl << std::endl;
        }

        std::cout << std::endl;
        subgroup_num++;
    }

}
