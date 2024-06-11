#include <stdlib.h>
#include <stdio.h>
#include<limits.h>
int n;            //n number of stations
# define MAX 100  // max number of station is 100


//struct named City
struct City
{
  int Serial_no;  // so that Station no. start with 1
  float Sky_view_factor;
  int Vegetation_density_ratio;
  int Population_density;
  int Pervious_surface_fraction;
  int Impervious_surface_fraction;
  char Status;
  int Distance[MAX];       
  int Temperature[30];    // 1 month =30 days
  float Relative_humidity[30];
};

struct City station[MAX];

void Heat_degree_days()
{
    int hdd[n];//storing number of heat degree days for n stations
    int count; //counting number of heat degree days
    float avg; //avg temp of all
    double total=0.0;// total temp for calculation of avg
    int order[n];//array to store sorted order of index A/c to no. of days
    int days;//temporary variable to sort
    int temp;//temp variable to swap

    for(int i=0;i<n;i++)                       //loop calculate avg
            {
                for(int j=0;j<30;j++)
                        {
                            total=total+station[i].Temperature[j];
//                            printf("%d  ",station[i].Temperature[j]);
                        }
            }
    avg=((total*1.0)/(30.0*n));
    printf("avg=%f total %f",avg,total);

    for(int i=0;i<n;i++)                //storing number of heat degree days
            {count=0;
                for(int j=0;j<30;j++)
                        {
                            if(station[i].Temperature[j]>avg)
                            {
                                count++;
                            }
                        }
             hdd[i] =count;             
            }
   
   for(int i=0;i<n;i++)                  //assigning order
    {
           order[i]=i;
    }
    for(int i=0;i<n;i++)                  //assigning order
    {  
        days=hdd[i];
        for(int j=i;j<n;j++)
            {
                if(hdd[j]>days)
                    {   temp=days;
                        days=hdd[j];
                        hdd[j]=temp;
                        temp=order[i];                        
                        order[i]=j;
                        order[j]=temp;
                    }            
            }
    }

    printf("order from highest to lowest is \n");    //printing order
    for(int i=0;i<n;i++)
    {
        printf("Station %d -  heat degree days= %d " ,station[order[i]].Serial_no,  hdd[order[i]]  );
    }

}



void Thermally_comfortable_days()
{   int s;
    printf("enter the station number");
    scanf("%d",&s );
    s--;             // convert station number to index
    if (station[s].Status=='r')
    {
        printf("rural station");
    }
    else
    {   int nearest[2];  // distance and index of station 
        nearest[0]=INT_MAX;

        for(int i=0;i<n;i++)                
            {
                if(station[s].Distance[i]<nearest[0] && i!=s && station[i].Status=='r')
                {
                    nearest[0]=station[s].Distance[i];
                    nearest[1]=i;
                }
            }
       
        int difference_temp[30];   //record difference each days
        for(int i=0;i<30;i++)                
            { 
                difference_temp[i]=abs(station[s].Temperature[i]-station[nearest[1]].Temperature[i]);
            }   

        
        int day_count=0;    
        int diff=0;         //starting from minimum difference 0
        int no;           // number of comfortable days required
        printf("enter number of suggestion days");
        scanf("%d",&no);
        printf("most Comfortable days are: \n");
        while(day_count<no && no>0)            // print n most comfortable days
        {
            for(int i=0;i<30;i++)                
            { 
                if(difference_temp[i]==diff)
                {
                    printf("Day %d ,",(i+1));
                    day_count++;
                }
            }   
            diff++;
        }       
    }
}



void Decreasing_vegetation()
{
    float avg; //avg temp of all
    float total=0.0;// total temp for calculation of avg
    int order[n];
    int temp;
    for(int i=0;i<n;i++)                       //loop calculate avg
            {
                for(int j=0;j<30;j++)
                        {
                            total=total+station[i].Temperature[j];
//                            printf("%d  ",station[i].Temperature[j]);
                        }
            }
    avg=((total*1.0)/(30.0*n));
    printf("avg=%f total %f",avg,total);
    
    for(int i=0;i<n;i++)                  //assigning order of veg density ratio
    { 
        order[i]=i;
    }
//    for(int i=0;i<n;i++)                  //assigning order of veg density ratio
//    { 
//        veg=station[i].Vegetation_density_ratio;
//	}
   for(int i=0;i<n;i++)                  //assigning order of veg density ratio
    {       
        for(int j=i+1;j<n;j++)
            {
                if(station[j].Vegetation_density_ratio>station[i].Vegetation_density_ratio)
                    {   
                        temp=order[i];
                        order[i]=order[j];
                        order[j]=temp;
                    }            
            }
    }

    for(int i=0;i<n;i++)                //  printing A/c veg den ratio
    {   
        printf("Station no- %d Veg density ratio- %d  days- ",station[order[i]].Serial_no,station[order[i]].Vegetation_density_ratio);
        for(int j=0;j<30;j++)
            {
                if(station[order[i]].Temperature[j]>avg)
                {
                    printf("%d, ",(j+1));
                }
            }
        printf("\n");
    }
}


void Minimum_population_density()
{
	printf("enter no of station k");
	int k;
	scanf("%d",&k);
    int min[n];    // stores index of those with minimum pop density
    int temp;
    int max_temp[n];

    for(int i=0;i<n;i++)                
    {   
        max_temp[i]=station[i].Temperature[0];
            for(int j=0;j<30;j++)
                    { 
                        if(station[i].Temperature[j]>max_temp[i])
                        {
                            max_temp[i]=station[i].Temperature[j];
                        }
                    }
    }

    for(int j=0;j<n;j++)  //initialise array min
            {
                min[j]=j;
            }

    for(int j=0;j<n;j++)  // storing in array min
            {
                for(int i=j;i<n;i++)                  
                {  
                    if(station[min[j]].Population_density> station[min[i]].Population_density)
                    {  
                        temp=min[j];
                        min[j]=min[i];
                        min[i]=temp;
                    }
                    else if(station[min[j]].Population_density==station[min[i]].Population_density && max_temp[min[i]]>max_temp[min[j]])
                    {
                        temp=min[j];
                        min[j]=min[i];
                        min[i]=temp;
                    }
                }

            }

    printf(" stations are:");
    for(int j=0;j<k;j++)  // printing
            {
                printf(" Station %d , " ,station[min[j]].Serial_no );
            }
    printf(" \n");
}

void Heat_island_intensity()
{
	float avg_temp[n];      // avg temp of all stations
    float total;
    for(int i=0;i<n;i++)  
    {
        total=0;
        for(int j=0;j<30;j++)
                {
                        total=total+station[i].Temperature[j];
                }
        avg_temp[i]=total/30;     
    }

    int nearest_dist=INT_MAX,nearest_rural=-1;
    float Heat_intensity;
    for(int i=0;i<n;i++)  
    {
        if(station[i].Status=='u')
        {
            for(int j=0;j<n;j++)  
            {
                if(station[j].Status=='r' && station[i].Distance[j]<nearest_dist)
                {
                    nearest_dist=station[i].Distance[j];
                    nearest_rural=j;
                }
                else if(station[j].Status=='r' && station[i].Distance[j]==nearest_dist && station[j].Pervious_surface_fraction>station[nearest_rural].Pervious_surface_fraction)
                {
                    nearest_dist=station[i].Distance[j];
                    nearest_rural=j;
                }
            }
        Heat_intensity=avg_temp[i]-avg_temp[nearest_rural];
        printf("Station %d Heat_island_intensity %f " ,station[i].Serial_no,Heat_intensity);
        }

    }
}
void main()
   {
   	FILE *myFile;
    myFile = fopen("dspd.txt", "r");
    printf("enter the number of climate stations \t");
    scanf("%d",&n);
    
    for(int i=0;i<n;i++)  // TAKING INPUT Data
    {
            station[i].Serial_no=i+1;
//            printf("enter details of station %d \n",station[i].Serial_no);         // check input type

//            printf("enter sky view factor "); 
            fscanf(myFile,"%f",&station[i].Sky_view_factor);
//            printf("enter vegetation density ratio");
            fscanf(myFile,"%d",&station[i].Vegetation_density_ratio);
//            printf("enter population density");
            fscanf(myFile,"%d",&station[i].Population_density);
//            printf("enter Pervious surface fraction ");
            fscanf(myFile,"%d",&station[i].Pervious_surface_fraction);
//            printf("enter Impervious surface fraction");
            fscanf(myFile,"%d",&station[i].Impervious_surface_fraction);
//            printf("enter Station status : r for rural ,u for urban");
			fscanf(myFile,"%s",&station[i].Status);

//            printf("enter Distance of station %d to other stations \n",station[i].Serial_no);

                for(int j=0;j<n;j++)
                    {
                       
                       if(j==i)
                       {
                           station[i].Distance[j]=0;
                       }
                       else 
                       {
//                            printf("enter distance from station %d \n",j);
                            fscanf(myFile,"%d", &station[i].Distance[j]);
                       }

                    }

                for(int j=0;j<30;j++)
                    {
//                            printf("enter temp on day (celcius) %d ",j);
                            fscanf(myFile,"%d", &station[i].Temperature[j]);
//                            printf("enter relative humidity on day %d ",j);
                            fscanf(myFile,"%f", &station[i].Relative_humidity[j]);
                       
                    }         
    }//input done
	fclose(myFile);
    int task=-1;
//    printf(" tem  %d"  ,station[0].Temperature[1] );
    while(task!=0)
    {
            printf("enter input :\n 1. List the stations with highest heat degree days.\n;2. For any searched station find the thermally comfortable days\n3. List dates for all stations with decreasing vegetation density ratio and for which the temperature was more than average station temperature.\n      4. List the  stations with minimum population density with highest maximum temperature.\n5. find the heat island intensity for all urban stations.\n0. EXIT ");
            scanf("%d", &task);
            if     (task==1)
            {
                Heat_degree_days();
            }
            else if(task==2)
            {
                Thermally_comfortable_days();
            }
            else if(task==3)
            {
                Decreasing_vegetation();
            }
            else if(task==4)
            {
                Minimum_population_density();
            }
            else if (task==5)
            {
                 Heat_island_intensity();
            }
            else if(task==0)
            {
                printf("task over");
            }
            else printf("enter valid input ");
            printf("\n");
    }

}
   
