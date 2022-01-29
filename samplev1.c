/* 
CO 222 Project 2
Author : Thulasiyan Y.


DID :
* Read csv file 
* count the frequency of each name and update to linkedlist (total duration and total participation also)
* linked list can be sort all three parameters
* print graph

NOTE :
* usage: ./samplev1 [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constants which are used in the code

#define SCALED 1
#define NONSCALED 0

#define FREQUENCY 101
#define DURATION 201
#define PARTICIPANTS 301

//--------------structure for store all csv file names-------------------------------

typedef struct file_node
{
    char file_name[50];
    struct file_node * next;
} file_node ;

file_node * file_head = NULL, *file_tail = NULL;


//--------------strucure for sort and total values of each names---------------------

typedef struct sort_total{
    char name[100];
    int  name_frequency;
    int  total_participants;
    int  total_duration;
    struct sort_total * next_link;
} total_t;

total_t * head_freq, * tail_freq, *totalHead;

//----------------------------Function prototypes--------------------------------------------------
int  durationToMinutes(char duration[]);
void print_freq_Values(int max, total_t * head_freq);
int  count_Node(total_t *head_count);
void swap(total_t *a, total_t *b);
int  max_lenght_of_name(int count);
int  total_of_parameter(int type, total_t * l_head);
int  lengthOfInt(unsigned x);
void draw_graph(int row_count, int scale_no, int parameter,total_t * head_draw,total_t * totalHead);

// ------Function prototypes for argument handling-------
int is_Integer(char str[]);
int csv_file_check(char *str);

//===========================================Main Function===================================================
int main(int argc, char **argv){

    //****************************************** Argument handling *******************************************
    
    // parameters which are we want to draw the graph with defoult values
    int bar_cout = 10;                      // bar_count is number of bars will be print in the graph (Eg : 10 names)
    int parameter = FREQUENCY;              // graph base on parameter
    int scale = 0;                         

    // variables which are going to use when argument handling
    int l_flag , p_flag , m_flag , t_flag , scaled_flag;
    int parameter_count = 0;
    int p_flag_count = 0 , m_flag_count = 0 , t_flag_count = 0 , scaled_flag_count = 0;
    int csvfile_count = 0;
    int valid_arg = 0;
    int l_flag_count = 0;
    int bar_input = 0;

    // If there is only one argument return "No input message" and exit
    if (argc == 1)
    {
        printf("No input files were given\n");
        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
        return 0;
    }

    // check all arguments one by one
    for (int i = 1; i < argc; i++)
    {
        // check whether this is a flag or not 
        if (strncmp(argv[i],"-",1) == 0)
        {
            valid_arg++;

            l_flag = strcmp(argv[i], "-l");
            p_flag = strcmp(argv[i], "-p");
            m_flag = strcmp(argv[i], "-m");
            t_flag = strcmp(argv[i], "-t");
            scaled_flag = strcmp(argv[i], "--scaled");
            
            // check whether current argument is valid flag or not
            if ( l_flag == 0 || p_flag == 0 ||  m_flag == 0 || t_flag == 0 || scaled_flag == 0)
            {
                // check whether it is length flag
                if (l_flag == 0)
                {
                    l_flag_count++;
                    if (i < argc-1)
                    {   
                        // check it is the valid form of length or not. if it is, assign length for bar_count
                        if (is_Integer(argv[i+1]) == 1)
                        {
                            bar_cout = atoi(argv[i+1]); 

                            // if bar count is 0, exit the program
                            if (bar_cout == 0)
                            {
                                return 0;
                            }
                            i++;
                            bar_input++;
                        }
                        // it is not a valid form of length print message and exit the program
                        else
                        {
                            printf("Invalid options for [%s]\n", argv[i]);
                            printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
                            return 0;
                        }
                        
                    }
                    // if -l is the last argument print the message and exit the program
                    else if(i == argc-1)
                    {
                        printf("Not enough options for [%s]\n", argv[i]);
                        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
                        return 0;
                    }
                    
                }
                // check -p -m -t flags. if it is, change the parameter to given parameter, and increase the flag vount by 1
                else if(p_flag == 0){
                    p_flag_count++;
                    parameter_count++;
                    parameter = PARTICIPANTS;
                }
                else if (m_flag == 0)
                {
                    m_flag_count++;
                    parameter_count++;
                    parameter = FREQUENCY;
                }
                else if (t_flag == 0)
                {
                    t_flag_count++;
                    parameter_count++;
                    parameter = DURATION;
                }
                else if (scaled_flag == 0)
                {
                    scaled_flag_count++;
                    scale = SCALED;
                }
                
            }
            // if argument is not a valid flag  print error mesage and exit the program
            else
            {
                printf("Invalid options for [%s]\n", argv[i]);
                printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
                return 0;
            }
            
        }
        // check it is the csv fil or not, if it is increase the csv file count by one assign csv file to a linked list
        if (csv_file_check(argv[i]) == 1 )
        {
            csvfile_count++;
            file_node *newNode = (file_node *)malloc(sizeof(file_node)); 
            strcpy(newNode->file_name,argv[i]);
            newNode->next = NULL;
            if (file_head == NULL)
            {
                file_head = newNode;
                file_tail = newNode;
            }
            else
            {
                file_tail->next = newNode;
                file_tail = newNode;
            }
        }   
    }
    // after check of the all argument, csv file count is equal to 0, print message and exit the program.
    if (csvfile_count == 0)
    {
        printf("No input files were given\n");
        printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
        return 0;
    }
    
    // parameter count is total parameter count (Eg: ./prog -m -p -m -t  , parameter count  = 4)
    if (parameter_count > 1)
    {   
        // check is there a two or more parameters, if it is print the message and exit thee program
        if ((parameter_count > t_flag_count && t_flag_count>0) || (parameter_count > p_flag_count && p_flag_count > 0) || (parameter_count > m_flag_count && m_flag_count>0))
        {
            printf("Cannot plot multiple parameters in same graph.\n");
            printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n", argv[0]);
            return 0;
        }
    }
    // check is there any non csv format file
    if (argc > (valid_arg + csvfile_count + 1 + bar_input))
    {
        printf("Only .csv files should be given as inputs.\n");
        return 0;
    }


    //****************************Reading CSV files and store in a linke list***********************************
    char buff[250];
    int column_no= 0;
    
    // temprary struct for save datas for a while.
    typedef struct _{
        char name[100];
        int participants;
        int duration;
    }temp_var;

    temp_var temp_file;

    // total hhead for save total counts for plot the graph
    total_t * totalHead = (total_t*)malloc(sizeof(total_t));
    totalHead->name_frequency = 0;
    totalHead->total_duration = 0;
    totalHead->total_participants = 0;
    totalHead->next_link = NULL;

    file_node * file_temp = file_head;
    while (file_temp != NULL)   // Iterating throught the input files
    {
        FILE *file_Ptr = fopen(file_temp->file_name,"r");
        if (!file_Ptr)
        {
            printf("Cannot open one or more given files\n");
            return 0;
        }

        // read csv file and store ina linked list and check is there any same name, if it is. update values
        int update_status = 0;
        static int name_count = 0;

        while (fgets(buff,1024,file_Ptr))
        {
            column_no = 0;  
            char *field = strtok(buff,",");
            while (field)
            {   
                if (column_no == 0)
                {
                    strcpy(temp_file.name,field);
                }
                if (column_no == 1)
                {
                    temp_file.participants = atoi(field);
                }
                if (column_no == 2)
                {
                    temp_file.duration= durationToMinutes(field);
                }
                field = strtok(NULL,",");
                column_no++;
            }
            totalHead->name_frequency++;
            totalHead->total_duration = totalHead->total_duration + temp_file.duration;
            totalHead->total_participants = totalHead->total_participants + temp_file.participants;

            if (name_count > 0)
            {
                for (total_t *current = head_freq; current != NULL ; current = current->next_link)
                {
                    if (strcmp(current->name,temp_file.name) == 0)
                    {
                        current->name_frequency++;
                        current->total_duration = current->total_duration + temp_file.duration;
                        current->total_participants = current->total_participants+temp_file.participants;

                        update_status = 1;
                        break;
                    }
                }
            }

            if (update_status == 0)
            {
                total_t * temporary = (total_t*)malloc(sizeof(total_t));
                strcpy(temporary->name,temp_file.name);
                temporary->name_frequency = 1;
                temporary->total_duration = temp_file.duration;
                temporary->total_participants = temp_file.participants;
                temporary->next_link = NULL;

                if (head_freq == NULL)
				{
					head_freq = temporary;
					tail_freq = temporary;
				}
                else
                {
                    tail_freq->next_link = temporary;
                    tail_freq = temporary;
                }
                name_count++;
            }
            else
            {
                update_status = 0;
            }
        }
    fclose(file_Ptr);
    file_temp = file_temp->next;
    }

    //********************************************************************************************************

    draw_graph(bar_cout,scale,parameter,head_freq,totalHead);

    //********************************************************************************************************

    return 0;
}
//---------------------------------Main Function end-----------------------------------------------------------




//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//=========================================== FUNCTIONS =======================================================
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::




//---------------- Functions for argument handling-----------------------

// check whether this srting contains digits or not
int is_Integer(char str[]){
    int i;
    for (i = 0; str[i]!= '\0'; i++){
        if (isdigit(str[i]) == 0)
              return 0;
    }
    return 1;
}

// check is this a csv format file
int csv_file_check(char *str){

    if (!strcmp(strrchr(str, '\0') - 4, ".csv")){
        return 1;
    }
    return 0;

}

//-------------------------Function for change the string duration to integer and count minutes----------------

int durationToMinutes(char duration[8]){
    int minutes;
    char *minStr = strtok(duration,":");
    int count = 0;
    int min, hour;
    while (minStr)
        {   
            if (count == 0)
            {
                hour = atoi(minStr);
            }
            if (count == 1)
            {
                min = atoi(minStr);            
            }
            minStr = strtok(NULL,":");
            count++;
        }
        minutes = (hour * 60) + min;
    return minutes;
}


//-----------------------------Function for count the nodes in a linked list -----------------------------------
int count_Node(total_t * head_count){

    total_t *count_node = (total_t*)malloc(sizeof(total_t));
    count_node =  head_count;
    int count = 0;

    while (count_node != NULL)
    {   
        count++;
        count_node = count_node->next_link;
    }
    return count;
}

//-------------------------Function for sort Data by frequency--------------------------------------------------
void sort_By_parameter(int parameter){
	total_t *node = head_freq;
	total_t *temp = NULL;

	if (head_freq == NULL)
		return;

    for( ; node != NULL ; )
    {
        temp=head_freq; 
        for ( ; temp->next_link != NULL ; )//travel till the second last element 
        {
            if (parameter == FREQUENCY)
            {
                if(temp->name_frequency < temp->next_link->name_frequency)// compare the data of the nodes 
                {
                    swap(temp,temp->next_link);
                }
            }
            else if (parameter == DURATION)
            {
                if(temp->total_duration < temp->next_link->total_duration)// compare the data of the nodes 
                {
                    swap(temp,temp->next_link);
                }
            }
            else if (parameter == PARTICIPANTS)
            {
                if(temp->total_participants < temp->next_link->total_participants)// compare the data of the nodes 
                {
                    swap(temp,temp->next_link);
                }
            }
           
         temp = temp->next_link;    // move to the next element 
        }
        node = node->next_link;    // move to the next node
    }
}

//-------------------------Function for swap while sorting------------------------------------------------------
void swap(total_t *a, total_t *b)
{
    total_t  * temprory = (total_t*)malloc(sizeof(total_t));

    strcpy(temprory->name,a->name);
    temprory->name_frequency = a->name_frequency;
    temprory->total_duration = a->total_duration;
    temprory->total_participants = a->total_participants;

    strcpy(a->name,b->name);
    a->name_frequency = b->name_frequency;
    a->total_duration = b->total_duration;
    a->total_participants = b->total_participants;

    strcpy(b->name,temprory->name);
    b->name_frequency = temprory->name_frequency;
    b->total_duration = temprory->total_duration;
    b->total_participants = temprory->total_participants;
}

//--------------------------Function for count the maximum length of  names-------------------------------------

int max_lenght_of_name(int count){
	total_t * lenTemp = head_freq;
    int node_count = count_Node(head_freq);

    if (count > node_count)
    {
        count = node_count;
    }
    
	int max_count = strlen(head_freq->name);
    for (int i = 0; i < count; i++)   
    {
        if (max_count < strlen(lenTemp->name))
        {
            max_count = strlen(lenTemp->name);
        }
        lenTemp = lenTemp->next_link;
    }
	return max_count;
}

//--------------------------Function for count the length of an integer-----------------------------------------
int lengthOfInt(unsigned x) {
    if (x >= 1000000000){
        return 10;
    }
    if (x >= 100000000){
        return 9;
    }
    if (x >= 10000000){
        return 8;
    }
    if (x >= 1000000){
        return 7;
    }
    if (x >= 100000){
        return 6;
    }
    if (x >= 10000){
        return 5;
    }
    if (x >= 1000){
        return 4;
    }
    if (x >= 100){
        return 3;
    }
    if (x >= 10){
        return 2;
    }
    else{
        return 1;
    }
}

//--------------------------Function for draw the graph for frequency-------------------------------------------

void draw_graph(int row_count, int scale_no, int parameter,total_t * head_draw,total_t * totalHead){
    //--------------sort data by given parameter------------------------
    if (parameter == FREQUENCY)
    {
        sort_By_parameter(FREQUENCY);
    }
    else if (parameter == DURATION)
    {
        sort_By_parameter(DURATION);
    }
    else if (parameter == PARTICIPANTS)
    {
        sort_By_parameter(PARTICIPANTS);
    }

    int maxLength = max_lenght_of_name(row_count);
    total_t *temp_graph = head_draw;
    float length = 0;
    int node_count = count_Node(head_freq);

    //------------If row count is grater than nodes count, assigned nodes count to row count-----------------------------
    if (row_count > node_count)
    {
        row_count = node_count;
    }

    //------------for loop for print each graph row one by one-----------------------------------------------------------
    for (int j = 0; j < row_count; j++)
    {
        //----------------select graph row length for every row for  given parameter-------------------------------------
        if (parameter == FREQUENCY)
        {
            //--------------select row length for scaled and non scaled use----------------------------------------------
            if (scale_no == 0)
            {
            length = (int)((temp_graph->name_frequency)*((float)(77-maxLength-lengthOfInt(head_draw->name_frequency)))/totalHead->name_frequency);
            }
            else if (scale_no == 1)
            {
            length = (int)((float)((77-maxLength-lengthOfInt(head_draw->name_frequency))*temp_graph->name_frequency)/(float)(head_draw->name_frequency));
            }
        }
        else if (parameter == DURATION)
        {
            if (scale_no == 0)
            {
            length = (int)((temp_graph->total_duration)*((float)(77-maxLength-lengthOfInt(head_draw->total_duration)))/totalHead->total_duration);
            }
            else if (scale_no == 1)
            {
            length = (int)((float)((77-maxLength-lengthOfInt(head_draw->total_duration))*temp_graph->total_duration)/(float)(head_draw->total_duration));
            }
        }
        else if (parameter ==  PARTICIPANTS)
        {
            if (scale_no == 0)
            {
            length = (int)((temp_graph->total_participants)*((float)(77-maxLength-lengthOfInt(head_draw->total_participants)))/totalHead->total_participants);
            }
            else if (scale_no == 1)
            {
            length = (int)((float)((77-maxLength-lengthOfInt(head_draw->total_participants))*temp_graph->total_participants)/(float)(head_draw->total_participants));
            }
        }
        //===============================================================================================================================
        //-------------print first line of a row------------------------------------
        printf("\n");
        for (int i = 0; i < maxLength +2 ; i++)
        {
            printf(" ");
        }  
        //-------print verical bar--------------------- 
        //printf("|");
        printf("\u2502");

        //-------print color for graph-----------------
        for(int j = 0; j < length ; j++){
            printf("\u2591");
        }
        printf("\n");

        //------------print name and 2nd line of row and parameter--------------------
        printf(" %s ",temp_graph->name);
        for (int i = 0; i < (maxLength - strlen(temp_graph->name) +1) -1; i++)
        {
            printf(" ");
        }
        //printf("|");
        printf("\u2502");
        for(int j = 0; j < length ; j++){
            printf("\u2591");
        }
        if (parameter == FREQUENCY)
        {
            printf("%d",temp_graph->name_frequency);
        }
        else if (parameter == DURATION)
        {
            printf("%d",temp_graph->total_duration);
        }
        else if (parameter == PARTICIPANTS)
        {
            printf("%d",temp_graph->total_participants);
        }
        printf("\n");

        ////-------------print third line of a row------------------------------------
        for (int i = 0; i < maxLength +2 ; i++)
        {
            printf(" ");
        }
        //-------print verical bar---------------------
        //printf("|");
        printf("\u2502");

        //-------print color for graph-----------------
        for(int j = 0; j < length ; j++){
            printf("\u2591");
        }
        printf("\n");

        //---------------print a space and vertical bar for space between two row bars----------------
        if(j != row_count-1)
        {      
            for (int i = 0; i < maxLength +2 ; i++)
            {
                printf(" ");
            }   
            //printf("|");
            printf("\u2502");
        }
        temp_graph = temp_graph->next_link;
    }   
    //----------------------print last horizontal line for the graph----------------------------------------
    for (int i = 0; i < maxLength +2 ; i++)
    {
        printf(" ");
    }   
    //printf("|");
    printf("\u2502");  
    printf("\n");
    for (int i = 0; i < 80 ; i++)
    {
        if (i < maxLength+2)
        {
            printf(" ");
        }
        else if (i == maxLength+2)
        {
            //printf("|");
            printf("\u2514");
        }
        else{
            //printf("_");
            printf("\u2500");
        } 
    }
    printf("\n");
    //-----------------------------graph drawing end---------------------------------------------------------
}
