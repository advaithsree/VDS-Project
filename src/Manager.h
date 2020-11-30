// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Written by Markus Wedler 2014

#ifndef mwBDD_H
#define mwBDD_H

#include <cassert>
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <map> 
#include <set>
#include <string>
#include "ManagerInterface.h"

namespace ClassProject {
    const BDD_ID ID_TRUE = 1;
    const BDD_ID ID_FALSE = 0;
    const BDD_ID LATEST_VALUE_INIT = 1;

    //classes for building own syntax-tree from function input, maybe move an own file in own folder.
    class abstract_snytax_tree_node {
    public:
        bool is_leaf() {
            return leaf;
        }

        abstract_snytax_tree_node* get_parent();
        abstract_snytax_tree_node* get_left_child();
        abstract_snytax_tree_node* get_right_child();

    private:
        abstract_snytax_tree_node * parent;
        abstract_snytax_tree_node * left_child;
        abstract_snytax_tree_node * right_child;

        std::string name;    //name of node, is either name of a function or of a variable
        bool leaf;  //if the node represents a variable then this will be true
    };




    class Abstract_syntax_tree {
    public:
        abstract_snytax_tree_node* get_root();
        std::string get_tree_name();

    private:
        std::string name;
        abstract_snytax_tree_node * root;
    };


    class Unique_identifier {
        public:
            Unique_identifier();        //identifier

            std::string top_var;
            BDD_ID id_low;
            BDD_ID id_high;
  
            // define "==" operation for unique_table_key
            bool operator==(const Unique_identifier &other) const
            { return (top_var == other.top_var
                        && id_low == other.id_low
                        && id_high == other.id_high);
            }
    };


    //end of classes for absract-syntax-tree handling
    class Unique_table_entry {
        public:
            Unique_table_entry();   //constructor


            Unique_identifier identifier;
            BDD_ID id;
            std::string label;
            //BDD_ID id_high;
            //BDD_ID id_low;
            //std::string top_var;
            bool is_variable;   //is true if the entry of this ID is a variable; false if not.
    };

}

/*
namespace std {
//define a hashing for our custom data-type ClassProject::Unique_table_key
  template <>
  struct hash<ClassProject::Unique_table_key>
  {
    std::size_t operator()(const ClassProject::Unique_table_key& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return ((hash<ClassProject::variable>()(k.top_var_name)
               ^ (hash<ClassProject::BDD_ID>()(k.id_low) << 1)) >> 1)
               ^ (hash<ClassProject::BDD_ID>()(k.id_high) << 1);
    }
  };

}
*/

namespace ClassProject {
    class ManagerImplementation {
    public:
        ManagerImplementation();    //Constructor
        void init(); //initialize ManagerImplementation


        //BDD_ID id_node_true  = ID_TRUE;   //ID of node representing TRUE always 1
        //BDD_ID id_node_false = ID_FALSE;   //ID of node representing TRUE always 0
        //std::unordered_map<Unique_table_key, Unique_table_entry> unique_table;  //our data-structure for the unique-table
        //std::unordered_map<BDD_ID, Unique_table_entry> unique_table;

        //is BDD_ID or (top_var,id_left,id_right) the key?!?!?
        //i need a data-structure with BDD_ID as the key!!!!!


        BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e);



        bool isVariable(const BDD_ID x);   //returns true if x is a variable
        bool isConstant(const BDD_ID f);    //returns true if f is a leaf-node
        BDD_ID topVar(const BDD_ID f);      //returns the ID of the top_variable of the node f

        BDD_ID createVar(const std::string &label);     //creates new variable for the BDD


        std::string getTopVarName(const BDD_ID &root);  //returns asghar the name of the top-variable of the node root



        const BDD_ID &True();
        const BDD_ID &False();


        size_t uniqueTableSize();   //returns the number of nodes currently exist in the table of the manager

        Unique_table_entry get_table_entry(BDD_ID x);
        BDD_ID add_table_entry(Unique_identifier identifier, std::string label);
        std::string get_top_var(BDD_ID x);


    private:
        BDD_ID latest_id_value;
        std::unordered_map<BDD_ID, Unique_table_entry> unique_table;
        BDD_ID id_node_true  = ID_TRUE;   //ID of node representing TRUE always 1
        BDD_ID id_node_false = ID_FALSE;   //ID of node representing TRUE always 0

        std::map<std::string, BDD_ID> variable_to_id_map;
        std::map<std::string, int> variable_to_order_map;   //start with a simple : variable first added has highest order

    };
}




#endif

// Local Variables:
// mode: c++
// End:




/*
    // Create an unordered_map of three strings (that map to strings)
    std::unordered_map<std::string, std::string> u = {
        {"RED","#FF0000"},
        {"GREEN","#00FF00"},
        {"BLUE","#0000FF"}
    };
 
    // Iterate and print keys and values of unordered_map
    for( const auto& n : u ) {
        std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
    }
 
    // Add two new entries to the unordered_map
    u["BLACK"] = "#000000";
    u["WHITE"] = "#FFFFFF";

*/