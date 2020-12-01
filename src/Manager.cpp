#include <cassert>

#include "Manager.h"


// Local Variables:
// mode: c++
// End:
    

namespace ClassProject {


    Unique_identifier::Unique_identifier(){
        id_low=-1;
        id_high=-1;
    }


    Unique_table_entry::Unique_table_entry(){
        Unique_identifier identifier;
        id=-1;
        is_variable=false;
    }


    ManagerImplementation::ManagerImplementation(){
    }






    BDD_ID ManagerImplementation::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e){
        //ite(i,t,e) = i*t + not(i)*e
        bool terminal_case = false;
        //case i) all 3 values are boolean then just evaluate i*t + not(i)*e
        if((i == 0 || i == 1) && (t == 0 || t == 1) && (e == 0 || e == 1)){
            bool i_b = (bool)i;
            bool t_b = (bool)t;
            bool e_b = (bool)e;

            bool ite_evaluation;
            ite_evaluation = i&&t || (!i)&&e;
            return (BDD_ID)ite_evaluation;
        }

        if((t == 0 || t == 1) && (e == 0 || e == 1)){   //case "if" is unknown and t and e are bool.
            if(t == 0 && e== 0) return ID_FALSE;
            if(t == 1 && e== 1) return ID_TRUE;
            if(t == 1 && e== 0) return i;
        }

        if((i == 0 || i == 1) && (e == 0 || e == 1)){   //case "then" is unknown and i and e are bool.
            if (i) return t;
            return e;// e is either 0 or 1
        }

        if((i == 0 || i == 1) && (t == 0 || t == 1)){   //case "else" is unknown and i and t are bool.
            if (!i) return e;
            return t;
            //if(t== 0) return ID_FALSE;
            //if(t== 1) return ID_TRUE;
        }


        //all terminal cases are checked!!!!

        //check cases that simplify e.g. ite(f,1,0) = f or ite(ID0, ID1, ID3) = ID3







        /*
        if(terminal_case){  //check if the input returns a boolean value
            //return a boolean value
        }else if(check if triple(i,t,e) has already_entry in table){
            //access the unique table and return result.
        }
        */



        //now check if triplet(top_variable, id_high=t, id_low=e) already exists in table
        //top variable is variable with least-ranking of our 3 ID's
        //get top variable of ID
        
        std::string top_var_priority;

        if (!(i == 0 || i == 1)){
            std::string top_var_i = get_top_var(i);
            top_var_priority = top_var_i;
        }
        
        if (!(t == 0 || t == 1)) {
            std::string top_var_t = get_top_var(t);
            if(variable_to_order_map[top_var_t] < variable_to_order_map[top_var_priority]){
                top_var_priority = top_var_t;
            }
        }

        if (!(e == 0 || e == 1)){
            std::string top_var_e = get_top_var(e);
            if(variable_to_order_map[top_var_e] < variable_to_order_map[top_var_priority]){
                top_var_priority = top_var_e;
            }
        }
        

        std::cout << "top_var_priority  " << top_var_priority << std::endl;



        //access top_variable through table
        Unique_identifier identifier;
        identifier.top_var = top_var_priority;
        identifier.id_high = t;
        identifier.id_low = e;

        std::pair<bool, BDD_ID> h;
        h = check_if_unique_identifier_in_table(identifier);

        if (h.first){
            std::cout << "case was detected of already exisiting identifier" << std::endl;
            return h.second;
        }


        BDD_ID i_high, i_low, t_high, t_low, e_high, e_low;

        if (get_top_var(i) == top_var_priority){
            i_high = get_table_entry(i).identifier.id_high;
            i_low = get_table_entry(i).identifier.id_low;
        } else{
            i_high = i;
            i_low =  i;
        }

        if (get_top_var(t) == top_var_priority){
            t_high = get_table_entry(t).identifier.id_high;
            t_low = get_table_entry(t).identifier.id_low;
        } else{
            t_high = t;
            t_low = t;
        }

        if (get_top_var(e) == top_var_priority){
            e_high = get_table_entry(e).identifier.id_high;
            e_low = get_table_entry(e).identifier.id_low;
        } else{
            e_high = e;
            e_low = e;
        }

        std::cout << "high_input:  " << i_high << "  "<< t_high << "  "  << e_high << std::endl;
        std::cout << "low_input:  " << i_low << "  "<< t_low << "  "  << e_low << std::endl;



        BDD_ID recursion_high, recursion_low;

        //recursion for high
        recursion_high = ite(i_high, t_high, e_high);
        std::cout << "recursion_high  " << recursion_high << std::endl;
        //recursion for low
        recursion_low = ite(i_low, t_low, e_low);
        std::cout << "recursion_low  " << recursion_low << std::endl;

        if (recursion_high == recursion_low) return recursion_high;


        //add result to unique_table: 
        identifier.top_var = top_var_priority;
        identifier.id_high = recursion_high;
        identifier.id_low = recursion_low;

        //check for duplicates before adding
        h = check_if_unique_identifier_in_table(identifier);

        if (h.first){
            std::cout << "case was detected of already exisiting identifier" << std::endl;
            return h.second;    //return the ID that corresponds to this entry
        }



        BDD_ID result_id;
        result_id = add_table_entry(identifier, "label");//improve label
        std::cout << " result_id  "  << result_id << " (identifier_top,recusion_high, recusion_low)  " << top_var_priority << " " << recursion_high <<  "  " << recursion_low << std::endl;

        return result_id;
    }


    const BDD_ID&  ManagerImplementation::True(){
        return id_node_true;
    }

    const BDD_ID&  ManagerImplementation::False(){
        return id_node_false;
    }

    bool ManagerImplementation::isConstant(const BDD_ID f){
        if(f == 0 || f == 1) return true;
        return false;
    }

    size_t ManagerImplementation::uniqueTableSize(){
        return unique_table.size();
    }

    bool ManagerImplementation::isVariable(const BDD_ID x){
        return unique_table[x].is_variable;
    }

    BDD_ID ManagerImplementation::createVar(const std::string &label){
        //construct new Unique_table_entry and add to unique_table
        latest_id_value++;
        
        Unique_identifier identifier;
        identifier.id_high = ID_TRUE;   //for a variable id_high has the value of the "True" node
        identifier.id_low  = ID_FALSE;  //for a variable id_high has the value of the "False" node
        identifier.top_var = label;


        Unique_table_entry new_entry;
        new_entry.id =latest_id_value;
        new_entry.label = label;
        new_entry.identifier = identifier;
        new_entry.is_variable = true;

        variable_to_order_map[label] = latest_id_value;
        variable_to_id_map[label] = latest_id_value;
        unique_table[latest_id_value] = new_entry;  //add new variable entry to the table
        
        return latest_id_value;
    }

    //not tested yet!
    BDD_ID ManagerImplementation::topVar(const BDD_ID f){      //returns the ID of the top_variable of the node f
        std::string top_variable = get_table_entry(f).identifier.top_var;
        std::cout << top_variable << std::endl;
        return variable_to_id_map[top_variable];
    }

    void ManagerImplementation::init(){

        //here initialize data-structure
        latest_id_value = LATEST_VALUE_INIT;
        BDD_ID id_node_true  = ID_TRUE;   //ID of node representing TRUE always 1
        BDD_ID id_node_false = ID_FALSE;   //ID of node representing TRUE always 0


        
        //add entry for TRUE into unique table
        Unique_identifier TRUE_identifier;
        TRUE_identifier.id_high = ID_TRUE;
        TRUE_identifier.id_low  = ID_TRUE;
        TRUE_identifier.top_var = "1";


        Unique_table_entry TRUE_entry;
        TRUE_entry.id =ID_TRUE;
        TRUE_entry.label = "1";
        TRUE_entry.identifier = TRUE_identifier;
        TRUE_entry.is_variable = false;


        auto true_entrie_pair = std::make_pair(ID_TRUE, TRUE_entry);
        unique_table.insert (true_entrie_pair);
        

        //unique_table[ID_TRUE] = TRUE_entry;  //add new variable entry to the table


        //add entry for FALSE into unique table
        Unique_identifier FALSE_identifier;
        FALSE_identifier.id_high = ID_FALSE;
        FALSE_identifier.id_low  = ID_FALSE;
        FALSE_identifier.top_var = "0";


        Unique_table_entry FALSE_entry;
        FALSE_entry.id =ID_FALSE;
        FALSE_entry.label = "0";
        FALSE_entry.identifier = FALSE_identifier;
        FALSE_entry.is_variable = false;

        //unique_table[ID_FALSE] = FALSE_entry;  //add new variable entry to the table

        auto false_entrie_pair = std::make_pair(ID_FALSE, FALSE_entry);
        unique_table.insert (false_entrie_pair);

    }




    BDD_ID ManagerImplementation::and2(const BDD_ID a, const BDD_ID b){
        return ite(a, b, ID_FALSE);
    }

    BDD_ID ManagerImplementation::or2(const BDD_ID a, const BDD_ID b){
        return ite(a, ID_TRUE, b);
    }

    BDD_ID ManagerImplementation::xor2(const BDD_ID a, const BDD_ID b){
        //xor(a,b) = a*not(b) + not(a)*b
        BDD_ID not_a = neg(a);
        BDD_ID not_b =neg(b);
        BDD_ID and_case_1 =and2(a, not_b);
        BDD_ID and_case_2 =and2(not_a, b);
        //return ite(ite(b, ID_FALSE, a), ID_TRUE, ite(a, ID_FALSE, b));
        return or2(and_case_1, and_case_2);
    }

    BDD_ID ManagerImplementation::neg(const BDD_ID a){
        return ite(a, ID_FALSE, ID_TRUE);
    }

    BDD_ID ManagerImplementation::nand2(const BDD_ID a, const BDD_ID b){
        //nand(a,b) = not(and(a,b))
        return neg(and2(a,b));
    }

    BDD_ID ManagerImplementation::nor2(const BDD_ID a, const BDD_ID b){
        return neg(or2(a,b));
    }



    BDD_ID ManagerImplementation::coFactorFalse(const BDD_ID f, BDD_ID x){
        return coFactorCase(f,x,false);
    }

      BDD_ID ManagerImplementation::coFactorTrue(const BDD_ID f, BDD_ID x){
        return coFactorCase(f,x,true);
    }

    BDD_ID ManagerImplementation::coFactorTrue(const BDD_ID f){
        Unique_table_entry node = get_table_entry(f);
        return node.identifier.id_high;
    }

    BDD_ID ManagerImplementation::coFactorFalse(const BDD_ID f){
        Unique_table_entry node = get_table_entry(f);
        return node.identifier.id_low;
    }

    BDD_ID ManagerImplementation::coFactorCase(const BDD_ID f, BDD_ID x, bool cofactor_case){
        //traverse through tree, when you encounter a node with ID(top_var)= x then replace node in parent with id_high
        //check if root_node is dependent on var x
        Unique_table_entry root = get_table_entry(f);
        std::string var_name = root.identifier.top_var;

        if (variable_to_id_map[var_name] == x)  {
            if (cofactor_case) {
                return root.identifier.id_high;
            }else {
                return root.identifier.id_low;
            }
        }

        std::set<BDD_ID> reachable_nodes;
        findNodes(f, reachable_nodes);
        BDD_ID replace_with_id;
        BDD_ID replaced_id;
        bool found_node_with_var = false;

        for (auto node_id : reachable_nodes) {
            Unique_table_entry node = get_table_entry(node_id);
            std::string var_name = node.identifier.top_var;
            if (variable_to_id_map[var_name] == x) {
                if (cofactor_case) {
                    replace_with_id = node.identifier.id_high;        

                }else {
                    replace_with_id = node.identifier.id_low;        
                }
                replaced_id = node_id;
                found_node_with_var = true;
                std::cout << "replaced_id,replace_with_id   "  << replaced_id  << "   "  <<  replace_with_id << std::endl;
            }
        }


        if (!found_node_with_var){
            std::cout << "Exit through not found" << std::endl;
            std::cout << f << std::endl;
            return f; //function is independent of variable x
        }


        for (auto node_id : reachable_nodes) {
            Unique_table_entry node = get_table_entry(node_id);
            if (node.identifier.id_high == replaced_id){
                node.identifier.id_high = replace_with_id;
                unique_table[node_id] = node;   //update node
            }
            if (node.identifier.id_low == replaced_id){
                node.identifier.id_low = replace_with_id;
                unique_table[node_id] = node;   //update node
            }
        }
        //now we have to check if there are nodes with id_high=id_low and thus can be replaced/removed.
        root = get_table_entry(f);
        if(root.identifier.id_high == root.identifier.id_low) {
            return root.identifier.id_high;
        }
        return f;
        }

    void ManagerImplementation::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root){    //returns the set of BDD nodes whih are reachable from the BDD node root(including itself)
        //traverse through "tree" starting at root.
        //go down until
        nodes_of_root.insert(root);
        if (root == ID_FALSE || root == ID_TRUE) return;
        Unique_table_entry node = get_table_entry(root);

        findNodes(node.identifier.id_low, nodes_of_root);
        findNodes(node.identifier.id_high, nodes_of_root);
    }


    std::string ManagerImplementation::getTopVarName(const BDD_ID &root){
        Unique_table_entry node = get_table_entry(root);
        return node.identifier.top_var;
    }





    void ManagerImplementation::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root){
        if (root == ID_FALSE || root == ID_TRUE) return;

        std::string var_name = getTopVarName(root);
        BDD_ID var_id = variable_to_id_map[var_name];
        vars_of_root.insert(var_id);
        Unique_table_entry node = get_table_entry(root);

        findVars(node.identifier.id_low, vars_of_root);
        findVars(node.identifier.id_high, vars_of_root);
    }




    Unique_table_entry ManagerImplementation::get_table_entry(BDD_ID x){
        return unique_table.at(x);
    }

    BDD_ID ManagerImplementation::add_table_entry(Unique_identifier identifier, std::string label){
        latest_id_value++;


        Unique_table_entry new_entry;
        new_entry.id =latest_id_value;
        new_entry.label = label;
        new_entry.identifier = identifier;
        new_entry.is_variable = false;

        unique_table[latest_id_value] = new_entry;  //add new variable entry to the table
        
        return latest_id_value;
    }


    std::pair<bool,BDD_ID> ManagerImplementation::check_if_unique_identifier_in_table(Unique_identifier x){
        //loop over all entries in table and check if we already hace this entry
        //first is the BDD_ID ; second is the table_entry
        for (std::pair<BDD_ID, Unique_table_entry> table_entry : unique_table){
            if (table_entry.second.identifier == x) return std::make_pair(true, table_entry.first);
        }
        return std::make_pair(false,-1);    //BDD_ID not of interes in this case
    }



    std::string ManagerImplementation::get_top_var(BDD_ID x){
        return unique_table.at(x).identifier.top_var;
    }

}
//begin with the parsing of the input function.
//input function has format : f = and(or(a,b),and(c,d)) then construct an Abstract-Syntax-Tree from it