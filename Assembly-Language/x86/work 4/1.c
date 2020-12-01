void insert_sort(int a[], int n)

{

    int i;

    int j, tmp;

    for(i = 1; i<n; i++)

    {

       tmp = a[i];

       j = i - 1;

       while(j >=0 && a[j] >tmp)

       {

          a[j+1] = a[j];

          j--;

       }

       a[j+1]=tmp;

    }

}
