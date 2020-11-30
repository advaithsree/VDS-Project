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

    //add result to unique_table: 
    identifier.top_var = top_var_priority;
    identifier.id_high = recursion_high;
    identifier.id_low = recursion_low;

    BDD_ID result_id;
    result_id = add_table_entry(identifier, "label");//improve label


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



std::string ManagerImplementation::get_top_var(BDD_ID x){
    return unique_table.at(x).identifier.top_var;
}

}
//begin with the parsing of the input function.
//input function has format : f = and(or(a,b),and(c,d)) then construct an Abstract-Syntax-Tree from it