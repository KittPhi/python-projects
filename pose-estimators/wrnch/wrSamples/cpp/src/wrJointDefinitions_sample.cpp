/*
 Copyright (c) 2019 wrnch Inc.
 All rights reserved
*/

/*
 This sample prints the available joint definitions (defined joints and defined bones) for hand and
 body poses.

 Key concepts:
    Joint definitions
    Joint definition registry

*/

#include <iostream>
#include <vector>

#include <wrnch/engine.hpp>

int main()
{
    std::vector<const char*> definitionList(wrnch::JointDefinitionRegistry::GetNumDefinitions());
    wrnch::JointDefinitionRegistry::GetAvailableDefinitions(definitionList.data());

    for (const char* definitionName : definitionList)
    {
        wrnch::JointDefinition definition = wrnch::JointDefinitionRegistry::Get(definitionName);
        definition.PrintJointDefinition();
    }

    return 0;
}
