//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"



//define a custom function to compare 2 Unique_identifier
void Unique_identifier_EQ(ClassProject::Unique_identifier id1, ClassProject::Unique_identifier id2){
    EXPECT_EQ(id1.top_var, id2.top_var);
    EXPECT_EQ(id1.id_low,  id2.id_low);
    EXPECT_EQ(id1.id_high, id2.id_high);
}


//define a custom function to compare 2 Unique_table_entry
void Unique_table_entry_EQ(ClassProject::Unique_table_entry entry1, ClassProject::Unique_table_entry entry2){
    Unique_identifier_EQ(entry1.identifier, entry2.identifier);
    EXPECT_EQ(entry1.id,  entry2.id);
    EXPECT_EQ(entry1.label, entry2.label);
    EXPECT_EQ(entry1.is_variable, entry2.is_variable);
}






struct ManagerTest: testing::Test {
    ManagerTest(){
        manager = ClassProject::ManagerImplementation ();
    }
    
    ClassProject::ManagerImplementation manager;
};



TEST(DataStructureTest, Test_IDs) {
    ClassProject::ManagerImplementation manager;
    ClassProject::Unique_identifier id1;
    ClassProject::Unique_identifier id2;
    manager.init();
    Unique_identifier_EQ(id1, id2);
}


TEST(DataStructureTest, Test_Entries) {
    ClassProject::Unique_table_entry entry1;
    ClassProject::Unique_table_entry entry2;
    Unique_table_entry_EQ(entry1, entry2);
}




TEST(InitializeTest, unique_table_size) {
    ClassProject::ManagerImplementation manager;
    manager.init();
    EXPECT_EQ(manager.uniqueTableSize(), 2);
}

TEST(InitializeTest, true_entry) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    ClassProject::Unique_identifier TRUE_identifier;
    TRUE_identifier.id_high = ClassProject::ID_TRUE;
    TRUE_identifier.id_low  = ClassProject::ID_TRUE;
    TRUE_identifier.top_var = "1";


    ClassProject::Unique_table_entry TRUE_entry;
    TRUE_entry.id = ClassProject::ID_TRUE;
    TRUE_entry.label = "1";
    TRUE_entry.identifier = TRUE_identifier;
    TRUE_entry.is_variable = false;



    Unique_table_entry_EQ(manager.get_table_entry(ClassProject::ID_TRUE), TRUE_entry);
}


TEST(InitializeTest, false_entry) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    ClassProject::Unique_identifier FALSE_identifier;
    FALSE_identifier.id_high = ClassProject::ID_FALSE;
    FALSE_identifier.id_low  = ClassProject::ID_FALSE;
    FALSE_identifier.top_var = "0";


    ClassProject::Unique_table_entry FALSE_entry;
    FALSE_entry.id = ClassProject::ID_FALSE;
    FALSE_entry.label = "0";
    FALSE_entry.identifier = FALSE_identifier;
    FALSE_entry.is_variable = false;



    Unique_table_entry_EQ(manager.get_table_entry(ClassProject::ID_FALSE), FALSE_entry);
}




TEST(ManagerTest, uniqueTableSizeTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    size_t s = manager.uniqueTableSize();
    manager.createVar("a");
    manager.createVar("b");
    manager.createVar("c");
    manager.createVar("d");
    manager.createVar("e");

    s = s + 5;

    EXPECT_EQ(manager.uniqueTableSize(), s);
}

TEST(ManagerTest, ID_TRUE_FALSE) {
    ClassProject::ManagerImplementation manager;
    manager.init();


    EXPECT_EQ(manager.True(), 1);
    EXPECT_EQ(manager.False(), 0);

}



TEST(ManagerTest, createVarTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    manager.createVar("a");

    ClassProject::Unique_identifier identifier_a;
    identifier_a.id_high = ClassProject::ID_TRUE;
    identifier_a.id_low  = ClassProject::ID_FALSE;
    identifier_a.top_var = "a";


    ClassProject::Unique_table_entry entry_a;
    entry_a.id = 2;
    entry_a.label = "a";
    entry_a.identifier = identifier_a;
    entry_a.is_variable = true;


    Unique_table_entry_EQ(manager.get_table_entry(2), entry_a);


    manager.createVar("b");

    ClassProject::Unique_identifier identifier_b;
    identifier_b.id_high = ClassProject::ID_TRUE;
    identifier_b.id_low  = ClassProject::ID_FALSE;
    identifier_b.top_var = "b";


    ClassProject::Unique_table_entry entry_b;
    entry_b.id = 3;
    entry_b.label = "b";
    entry_b.identifier = identifier_b;
    entry_b.is_variable = true;


    Unique_table_entry_EQ(manager.get_table_entry(3), entry_b);

}

/*
TEST(ManagerTest, topVarTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    manager.createVar("a");

    ClassProject::Unique_identifier identifier_a;
    identifier_a.id_high = ClassProject::ID_TRUE;
    identifier_a.id_low  = ClassProject::ID_FALSE;
    identifier_a.top_var = "a";


    ClassProject::Unique_table_entry entry_a;
    entry_a.id = 2;
    entry_a.label = "a";
    entry_a.identifier = identifier_a;
    entry_a.is_variable = true;


    manager.createVar("b");

    ClassProject::Unique_identifier identifier_b;
    identifier_b.id_high = ClassProject::ID_TRUE;
    identifier_b.id_low  = ClassProject::ID_FALSE;
    identifier_b.top_var = "a";


    ClassProject::Unique_table_entry entry_b;
    entry_b.id = 3;
    entry_b.label = "b";
    entry_b.identifier = identifier_b;
    entry_b.is_variable = true;


    EXPECT_EQ(manager.topVar(3), 2);

}
*/


TEST(ManagerTest, iteTermincalCasesTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    //test the pure boolean cases first
    EXPECT_EQ(manager.ite(0,0,0), 0);
    EXPECT_EQ(manager.ite(1,0,0), 0);
    EXPECT_EQ(manager.ite(0,1,0), 0);
    EXPECT_EQ(manager.ite(1,1,0), 1);
    EXPECT_EQ(manager.ite(0,0,1), 1);
    EXPECT_EQ(manager.ite(1,0,1), 0);
    EXPECT_EQ(manager.ite(0,1,1), 1);
    EXPECT_EQ(manager.ite(1,1,1), 1);


    //test cases i is unknown(variable or sth else), t,e are boolean
    EXPECT_EQ(manager.ite(2,0,0), 0);
    EXPECT_EQ(manager.ite(2,1,1), 1);


    //test cases t is unknown(variable or sth else), i,e are boolean
    EXPECT_EQ(manager.ite(0,2,0), 0);
    EXPECT_EQ(manager.ite(0,2,1), 1);


    //test cases e is unknown(variable or sth else), i,t are boolean
    EXPECT_EQ(manager.ite(1,0,2), 0);
    EXPECT_EQ(manager.ite(1,1,2), 1);


    //test cases where ite returns one of its inputs
    EXPECT_EQ(manager.ite(0,1,3), 3);
    EXPECT_EQ(manager.ite(0,0,3), 3);
    EXPECT_EQ(manager.ite(1,5,0), 5);
    EXPECT_EQ(manager.ite(5,1,0), 5);
    EXPECT_EQ(manager.ite(1,5,1), 5);
}


TEST(ManagerTest, iteORTest) {
    ClassProject::ManagerImplementation manager;
    manager.init();

    manager.createVar("a");
    ClassProject::Unique_identifier identifier_a;
    identifier_a.id_high = ClassProject::ID_TRUE;
    identifier_a.id_low  = ClassProject::ID_FALSE;
    identifier_a.top_var = "a";


    ClassProject::Unique_table_entry entry_a;
    entry_a.id = 2;
    entry_a.label = "a";
    entry_a.identifier = identifier_a;
    entry_a.is_variable = true;


    Unique_table_entry_EQ(manager.get_table_entry(2), entry_a);


    manager.createVar("b");

    ClassProject::Unique_identifier identifier_b;
    identifier_b.id_high = ClassProject::ID_TRUE;
    identifier_b.id_low  = ClassProject::ID_FALSE;
    identifier_b.top_var = "b";


    ClassProject::Unique_table_entry entry_b;
    entry_b.id = 3;
    entry_b.label = "b";
    entry_b.identifier = identifier_b;
    entry_b.is_variable = true;


    //or(a,b) = ite(a, 1, b)
    ClassProject::Unique_table_entry result_entry = manager.get_table_entry(4);

    ClassProject::Unique_identifier identifier_result;
    identifier_result.id_high = ClassProject::ID_TRUE;
    identifier_result.id_low  = entry_b.id;
    identifier_result.top_var = "a";
    //(a,1,3)

    manager.ite(entry_a.id, 1, entry_b.id);
    Unique_identifier_EQ(manager.get_table_entry(4).identifier, identifier_result);

}









#endif //VDS_PROJECT_TESTS_H

