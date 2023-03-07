#include "ast.hpp"

#include <regex>

using namespace std;

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());
        
    // TODO : Check for things matching reId
    }else if(regex_match(program->type ,reId)){

        auto value=context.bindings.at(program->type);
        return value;
        
    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
    
    }else if(program->type=="Input"){
        int32_t val;
        std::cin >> val;
        return val;
    
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
        
    // TODO: Handle other constructs
    }else if (program->type=="Assign"){
        int32_t value = Interpret(context, program->branches.at(0));
        context.bindings[program->value] = value;
        return value;
    
    }else if(program->type=="Add"){
        int32_t left = Interpret (context, program->branches.at(0));
        int32_t right= Interpret (context, program->branches.at(1));
        return left+right;
    }

    else if(program->type=="Sub"){
        int32_t left=Interpret(context, program->branches.at(0));
        int32_t right=Interpret(context, program->branches.at(1));
        return left-right;
    }

    else if(program->type=="LessThan"){
        int32_t left=Interpret(context, program->branches.at(0));
        int32_t right=Interpret(context, program->branches.at(1));
        return left<right;
    }

    else if(program->type=="Seq"){
        int32_t result = 0;
        for(int i=0;i<program->branches.size();i++){
            result = Interpret(context, program->branches.at(i));
        }
        return result;
    }

    else if(program->type=="While"){
        auto cond = program->branches.at(0);
        auto stat = program->branches.at(1);
        while(Interpret(context, cond)){
            Interpret(context, stat);
        }
        return 0;
    }

    else if(program->type=="If"){
        auto cond = program->branches.at(0);
        auto stat1 = program->branches.at(1);
        auto stat2 = program->branches.at(2);
        if( Interpret(context, cond)){

            return Interpret(context, stat1);
        }
        else{

            return Interpret(context, stat2);
        }
    }
  
    else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
