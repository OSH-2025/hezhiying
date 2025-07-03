#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <deque>

#define MAX_SVC_NAME 256
#define MAX_PARAM_NAME 256
#define MAX_PARAM_TYPE 256

using namespace std;

struct svcRoutine
{
    int id;
    string name;

    struct svcParam
    {
        string name;
        string type;
    };

    deque<svcParam> params;
};

int main()
{
    printf("This is SVC routine compiler, built " __DATE__ ", " __TIME__ "\n");

    FILE *svcDefFile = fopen("lightrt/syscall/svc.def", "r");

    int nSvcs = 0;
    fscanf(svcDefFile, "%d", &nSvcs);

    printf("Total SVC call definitions: %d\n", nSvcs);

    deque<svcRoutine> routines;

    for (int i = 0; i < nSvcs; ++i)
    {
        svcRoutine routine;

        char svcName[MAX_SVC_NAME] = {0};
        int svcId = 0, nParams = 0;

        fscanf(svcDefFile, "%s%d%d", svcName, &svcId, &nParams);
        printf("SVC %d name: %s, %d params:\n", svcId, svcName, nParams);

        routine.id = svcId;
        routine.name = svcName;

        for (int j = 0; j < nParams; ++j)
        {
            svcRoutine::svcParam param;

            char paramName[MAX_PARAM_NAME] = {0};
            char paramType[MAX_PARAM_TYPE] = {0};

            fscanf(svcDefFile, "%s %[^\n]", paramName, paramType);
            printf("  Parameter %d: %s of type %s\n", j, paramName, paramType);

            param.name = paramName;
            param.type = paramType;

            routine.params.push_back(param);
        }

        routines.push_back(routine);
    }

    fclose(svcDefFile);
    printf("Done parsing all SVCs.\n");

    for (int i = 0; i < routines.size(); ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            if (routines[i].id == routines[j].id)
            {
                fprintf(stderr, "Conflict SVC ID [%d]%s and [%d]%s.\n", routines[i].id, routines[i].name.c_str(), routines[j].id, routines[j].name.c_str());
                abort();
            }

            if (routines[i].name == routines[j].name)
            {
                fprintf(stderr, "Conflict SVC name [%d]%s and [%d]%s.\n", routines[i].id, routines[i].name.c_str(), routines[j].id, routines[j].name.c_str());
                abort();
            }
        }
    }

    printf("Generating SVC call user header...\n");
    FILE *userHeaderFile = fopen("lightrt/user/syscall.h", "w");

    fprintf(userHeaderFile, "#include \"../lightrt.h\"\n");
    fprintf(userHeaderFile, "#ifndef _U_SYSCALL_H\n#define _U_SYSCALL_H\n");
    fprintf(userHeaderFile, "/* This file is generated using SVC compiler. DO NOT DIRECTLY MODIFY. Edit svc.def file and run svcc instead. */\n");

    for (auto &routine : routines)
    {
        fprintf(userHeaderFile, "/* SVC Call %d */ __attribute__((naked)) uint32_t lrt_%s (", routine.id, routine.name.c_str());
        for (int i = 0; i < routine.params.size(); ++i)
        {
            fprintf(userHeaderFile, "%s %s%s",
                    routine.params[i].type.c_str(),
                    routine.params[i].name.c_str(),
                    i == routine.params.size() - 1 ? "" : ", ");
        }
        fprintf(userHeaderFile, ");\n");
    }

    fprintf(userHeaderFile, "#endif");
    fclose(userHeaderFile);

    printf("Generating SVC call user mode source...\n");
    FILE *userSourceFile = fopen("lightrt/user/syscall.c", "w");

    fprintf(userSourceFile, "/* This file is generated using SVC compiler. DO NOT DIRECTLY MODIFY. Edit svc.def file and run svcc instead. */\n");
    fprintf(userSourceFile, "#include \"syscall.h\"\n");
    for (auto &routine : routines)
    {
        fprintf(userSourceFile, "/* Entry for SVC Call %d */ __attribute__((naked)) uint32_t lrt_%s (", routine.id, routine.name.c_str());
        for (int i = 0; i < routine.params.size(); ++i)
        {
            fprintf(userSourceFile, "%s %s%s",
                    routine.params[i].type.c_str(),
                    routine.params[i].name.c_str(),
                    i == routine.params.size() - 1 ? "" : ", ");
        }
        fprintf(userSourceFile, ")\n");

        fprintf(userSourceFile, "{\n    __asm volatile (\n");
        for (int i = 0; i < routine.params.size(); ++i)
        {
            fprintf(userSourceFile, "        \"mov r%d, %%%d \\n\"\n", i, i);
        }

        fprintf(userSourceFile, "        \"svc #%d \\n\"\n", routine.id);
        fprintf(userSourceFile, "        \"bx lr \\n\"\n");

        if (routine.params.size() > 0)
        {
            fprintf(userSourceFile, "        :\n");
            fprintf(userSourceFile, "        :");

            for (int i = 0; i < routine.params.size(); ++i)
            {
                fprintf(userSourceFile, " \"r\" (%s)%s", routine.params[i].name.c_str(), i == routine.params.size() - 1 ? "\n" : ",");
            }

            fprintf(userSourceFile, "        :");
            for (int i = 0; i < routine.params.size(); ++i)
            {
                fprintf(userSourceFile, " \"r%d\"%s", i, i == routine.params.size() - 1 ? "\n" : ",");
            }
        }

        fprintf(userSourceFile, "    );\n");
        fprintf(userSourceFile, "}\n");
    }

    fclose(userSourceFile);

    printf("Generating SVC call kernel mode header...\n");
    FILE *kernelHeaderFile = fopen("lightrt/syscall/svc.h", "w");

    fprintf(kernelHeaderFile, "#include \"../lightrt.h\"\n");
    fprintf(kernelHeaderFile, "#ifndef _SVC_H\n#define _SVC_H\n");
    fprintf(kernelHeaderFile, "/* This file is generated using SVC compiler. DO NOT DIRECTLY MODIFY. Edit svc.def file and run svcc instead. */\n");
    fprintf(kernelHeaderFile, "#include <stdint.h>\n");

    for (auto &routine : routines)
    {
        fprintf(kernelHeaderFile, "/* SVC Implement %d */ uint32_t svc_%s (", routine.id, routine.name.c_str());
        for (int i = 0; i < routine.params.size(); ++i)
        {
            fprintf(kernelHeaderFile, "%s %s%s",
                    routine.params[i].type.c_str(),
                    routine.params[i].name.c_str(),
                    i == routine.params.size() - 1 ? "" : ", ");
        }
        fprintf(kernelHeaderFile, ");\n");
    }

    fprintf(kernelHeaderFile, "#endif");
    fclose(kernelHeaderFile);

    printf("Generating SVC call kernel mode source...\n");
    FILE *kernelSourceFile = fopen("lightrt/syscall/svc.c", "w");

    fprintf(kernelSourceFile, "/* This file is generated using SVC compiler. DO NOT DIRECTLY MODIFY. Edit svc.def file and run svcc instead. */\n");
    fprintf(kernelSourceFile, "#include <stdint.h>\n");
    fprintf(kernelSourceFile, "#include \"svc.h\"\n");

    fprintf(kernelSourceFile, "void svcHandler(uint32_t *svc_args)\n"
                              "{\n"
                              "    uint8_t *svc_instr = (uint8_t *)(svc_args[6] - 2);\n"
                              "    uint8_t svc_number = *svc_instr & 0xFF;\n"
                              "    switch (svc_number)\n"
                              "    {\n");

    for (auto &routine : routines)
    {
        fprintf(kernelSourceFile, "        case %d:\n", routine.id);
        fprintf(kernelSourceFile, "            svc_args[0] = svc_%s(\n", routine.name.c_str());
        for (int i = 0; i < routine.params.size(); ++i)
        {
            fprintf(kernelSourceFile, "                (%s)(svc_args[%d])%s", routine.params[i].type.c_str(), i, i == routine.params.size() - 1 ? "\n" : ", \n");
        }
        fprintf(kernelSourceFile, "            );\n            break;\n");
    }

    fprintf(kernelSourceFile, "    }\n}");

    fclose(kernelSourceFile);
}