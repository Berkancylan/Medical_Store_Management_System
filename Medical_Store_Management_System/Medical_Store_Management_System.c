#include <stdio.h>
#include <stdlib.h>
#include <cJSON.c>

cJSON *dosyadan_okuma_ilaclar()
{
    FILE *file = fopen("ilaclar.json", "r");
    cJSON *root;

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (fileSize > 0)
        {
            char *fileContent = (char *)malloc(fileSize + 1);
            fread(fileContent, 1, fileSize, file);
            fileContent[fileSize] = '\0';

            root = cJSON_Parse(fileContent);
            free(fileContent);
        }
        else
        {
            root = cJSON_CreateArray();
        }

        fclose(file);
    }
    else
    {
        root = cJSON_CreateArray();
    }
    return root;
}

cJSON *dosyadan_okuma_kisiler()
{
    FILE *file = fopen("kisiler.json", "r");
    cJSON *root;

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (fileSize > 0)
        {
            char *fileContent = (char *)malloc(fileSize + 1);
            fread(fileContent, 1, fileSize, file);
            fileContent[fileSize] = '\0';

            root = cJSON_Parse(fileContent);
            free(fileContent);  
        }
        else
        {
            root = cJSON_CreateArray();
        }

        fclose(file);
    }
    else
    {
        root = cJSON_CreateArray();
    }
    return root;
}

void search_person(long long int search_TC)
{
    cJSON *root = dosyadan_okuma_kisiler();
    cJSON *current = root->child;

    while (current != NULL)
    {
        cJSON *person_TC = cJSON_GetObjectItem(current, "TC");
        long long int currentTC = atoll(cJSON_Print(person_TC));
        if (currentTC == search_TC)
        {
            printf("Name: %s\n", cJSON_GetObjectItem(current, "name")->valuestring);
            printf("Surname: %s\n", cJSON_GetObjectItem(current, "surname")->valuestring);
            printf("TC: %lld\n", currentTC);

            cJSON *medicineCodeArray = cJSON_GetObjectItem(current, "medicine_code");
            int arraySize = cJSON_GetArraySize(medicineCodeArray);
            int i;
            printf("Medicine Code: ");
            for (i = 0; i < arraySize; i++)
            {
                cJSON *medicineCodeItem = cJSON_GetArrayItem(medicineCodeArray, i);
                printf("%d ", medicineCodeItem->valueint);
            }
            printf("\n\n");
            break;
        }
        current = current->next;
    }
    cJSON_Delete(root);
}

void create_person()
{
    char name[50], surname[50];
    long long int TC;
    int received_medicine_codes;

    printf("Person name: ");
    scanf("%s", name);

    printf("Person surname: ");
    scanf("%s", surname);

    printf("TC: ");
    scanf("%lld", &TC);

    printf("Medicine code: ");
    scanf("%d", &received_medicine_codes);

    cJSON *root = dosyadan_okuma_kisiler();

    cJSON *person = cJSON_CreateObject();
    cJSON_AddStringToObject(person, "name", name);
    cJSON_AddStringToObject(person, "surname", surname);
    cJSON_AddNumberToObject(person, "TC", TC);

    cJSON *medicineCodesArray = cJSON_CreateArray();
    cJSON_AddItemToArray(medicineCodesArray, cJSON_CreateNumber(-1));
    cJSON_AddItemToObject(person, "medicine_code", medicineCodesArray);
    cJSON_AddItemToArray(root, person);

    FILE *writeFile = fopen("kisiler.json", "w");

    fprintf(writeFile, "%s\n", cJSON_Print(root));
    fclose(writeFile);

    cJSON_Delete(root);
    printf("\n");
}

void edit_person_function(long long int search_TC, int secim)
{
    long long int new_TC;

    cJSON *root = dosyadan_okuma_kisiler();
    cJSON *current = root->child;

    while (current != NULL)
    {
        cJSON *person_TC = cJSON_GetObjectItem(current, "TC");
        long long int currentTC = atoll(cJSON_Print(person_TC));
        if (currentTC == search_TC)
        {
            switch (secim)
            {
            case 1://Some parts were not written
                break;

            case 2:
                break;

            case 3:
                printf("New TC: ");
                scanf("%lld", &new_TC);

                cJSON *amountItem = cJSON_CreateNumber(new_TC);
                cJSON_ReplaceItemInObject(current, "TC", amountItem);
                FILE *writeFile = fopen("kisiler.json", "w");

                fprintf(writeFile, "%s\n", cJSON_Print(root));
                fclose(writeFile);

                break;

            case 4:
                break;

            default:
                break;
            }
            break;
        }
        current = current->next;
    }
    cJSON_Delete(root);
}

void edit_person()
{
    long long int search_TC;
    int secim;
    printf("TC: ");
    scanf("%lld", &search_TC);

    search_person(search_TC);

    printf("Name change(1)\nSurname change(2)\nTC change(3)\nMedicine code change(4)\nExit (0)\n");
    scanf("%d", &secim);

    while (secim == 1 || secim == 2 || secim == 3 || secim == 4)
    {
        edit_person_function(search_TC, secim);
        break;
    }
}

void search_medicine(int search_code)
{
    cJSON *root = dosyadan_okuma_ilaclar();
    cJSON *current = root->child;

    while (current != NULL)
    {
        cJSON *medicineCodeItem = cJSON_GetObjectItem(current, "medicine_code");
        int currentCode = medicineCodeItem->valueint;
        if (currentCode == search_code)
        {
            printf("Name: %s\n", cJSON_GetObjectItem(current, "name")->valuestring);
            printf("Amount: %d\n", cJSON_GetObjectItem(current, "amount")->valueint);
            printf("Medicine Code: %d\n\n", currentCode);
            break;
        }
        current = current->next;
    }

    cJSON_Delete(root);
}

void satilan_ilaci_tc_isleme(long long int TC, int search_code)
{
    cJSON *root = dosyadan_okuma_kisiler();
    cJSON *current = root->child;

    while (current != NULL)
    {
        cJSON *personCodeItem = cJSON_GetObjectItem(current, "TC");
        long long int currentCode = atoll(cJSON_Print(personCodeItem)); // atoll bak

        if (currentCode == TC)
        {
            cJSON *medicineCodesArray = cJSON_GetObjectItem(current, "medicine_code");
            cJSON_AddItemToArray(medicineCodesArray, cJSON_CreateNumber(search_code));

            FILE *writeFile = fopen("kisiler.json", "w");

            fprintf(writeFile, "%s\n", cJSON_Print(root));
            fclose(writeFile);
            break;
        }
        current = current->next;
    }

    cJSON_Delete(root);
}

void sale_medicine()
{
    long long int TC;
    int search_code, deducted_amount;

    printf("The ID of the person taking the medicine: ");
    scanf("%lld", &TC);

    printf("The code of the medicine to be sold : ");
    scanf("%d", &search_code);

    search_medicine(search_code);

    printf("How many boxes of medicine were sold: ");
    scanf("%d", &deducted_amount);

    cJSON *root = dosyadan_okuma_ilaclar();
    cJSON *current = root->child;

    while (current != NULL)
    {
        cJSON *medicineCodeItem_code = cJSON_GetObjectItem(current, "medicine_code");
        cJSON *medicineCodeItem_amount = cJSON_GetObjectItem(current, "amount");

        int current_Code = medicineCodeItem_code->valueint;
        int current_amount = medicineCodeItem_amount->valueint;
        int new_amount = current_amount - deducted_amount;

        if (current_Code == search_code)
        {
            if (new_amount < 0)
            {
                if (current_amount == 0)
                {
                    printf("No medicine in stock\n\n");
                    break;
                }
                else
                {
                    printf("Maximum number of boxes you can buy %d\n\n", current_amount);
                    break;
                }
            }
            else
            {
                cJSON *amountItem = cJSON_CreateNumber(new_amount);
                cJSON_ReplaceItemInObject(current, "amount", amountItem);
                FILE *writeFile = fopen("ilaclar.json", "w");

                fprintf(writeFile, "%s\n", cJSON_Print(root));
                fclose(writeFile);

                satilan_ilaci_tc_isleme(TC, search_code);
                break;
            }
        }
        current = current->next;
    }

    cJSON_Delete(root);
}

void stock_change()
{
    int search_code, incoming_medicine;

    printf("Medicine code : ");
    scanf("%d", &search_code);

    search_medicine(search_code);

    printf("Number of incoming medicines (boxes) : ");
    scanf("%d", &incoming_medicine);

    cJSON *root = dosyadan_okuma_ilaclar();
    cJSON *current = root->child;

    while (current != NULL)
    {
        cJSON *medicineCodeItem_code = cJSON_GetObjectItem(current, "medicine_code");
        cJSON *medicineCodeItem_amount = cJSON_GetObjectItem(current, "amount");

        int current_Code = medicineCodeItem_code->valueint;
        int current_amount = medicineCodeItem_amount->valueint;
        int new_amount = current_amount + incoming_medicine;

        if (current_Code == search_code)
        {
            cJSON *amountItem = cJSON_CreateNumber(new_amount);
            cJSON_ReplaceItemInObject(current, "amount", amountItem);
            FILE *writeFile = fopen("ilaclar.json", "w");

            fprintf(writeFile, "%s\n", cJSON_Print(root));
            fclose(writeFile);

            break;
        }
        current = current->next;
    }

    cJSON_Delete(root);
}

void purchase_new_medicine()
{
    char name[50];
    int amount, code;

    printf("Medicine name: ");
    scanf("%s", name);

    printf("Amount (mg): ");
    scanf("%d", &amount);

    printf("Medicine code: ");
    scanf("%d", &code);

    cJSON *root = dosyadan_okuma_ilaclar();

    cJSON *person = cJSON_CreateObject();
    cJSON_AddStringToObject(person, "name", name);
    cJSON_AddNumberToObject(person, "amount", amount);
    cJSON_AddNumberToObject(person, "medicine_code", code);
    cJSON_AddItemToArray(root, person);

    FILE *writeFile = fopen("ilaclar.json", "w");

    fprintf(writeFile, "%s\n", cJSON_Print(root));
    fclose(writeFile);

    cJSON_Delete(root);
    printf("\n");
}

void medicine()
{
    int secim = 1, search_code;

    while (secim != 0)
    {
        printf("Search Medicine(1)\nSale Medicine(2)\nStock_change(3)\nPurchase New Medicine(4)\nMain Menu(0)\n");
        scanf("%d", &secim);

        switch (secim)
        {
        case 1:
            printf("Aranan ilac kodu : ");
            scanf("%lld", &search_code);
            search_medicine(search_code);
            break;
        case 2:
            sale_medicine();
            break;
        case 3:
            stock_change();
            break;
        case 4:
            purchase_new_medicine();
            break;

        default:
            break;
        }
    }
}

void supplier_info()
{
    int secim = 1;
    long long int TC;

    while (secim != 0)
    {
        printf("Search person(1)\nAdd person(2)\nEdit person(3)\nExit(0)\n");
        scanf("%d", &secim);

        switch (secim)
        {
        case 1:
            printf("TC: ");
            scanf("%lld", &TC);
            search_person(TC);
            break;

        case 2:
            create_person();
            break;
        case 3:
            edit_person();
            break;

        default:
            break;
        }
    }
}

int main(void)
{
    int secim = 1;

    while (secim != 0)
    {
        printf("Supplier info(1)\nMedicine(2)\nExit(0)\n");
        scanf("%d", &secim);

        switch (secim)
        {
        case 1:
            supplier_info();
            break;
        case 2:
            medicine();
            break;
        default:
            break;
        }
    }
    return 0;
}