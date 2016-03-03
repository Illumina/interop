# Performance                                               {#perf}

The InterOp library is not optimized for speed. Nonetheless, the user should be aware of certain downsides to
using the SWIG interface to C# (or any other language).

## SWIG Binding Performance

The C# unit tests include a PerformanceTest.cs. This file shows three ways to simply sum all the focus scores
for the first channel over >5 million entries. 

The PerformanceTest.cs class demonstrates three different ways of summing focus scores:

### 2. Direct Call 1

The main loop in is C#

```csharp
double sum = 0.0;
for(uint i=0;i<extraction_metric_set.size();i++)
sum += extraction_metric_set.at(i).focusScore(0);
```

### 3. Direct Call 2

The main loop in is C#

```csharp

double sum = 0.0;
for(int lane = 1;lane <=8;lane++)
{
    for(int tile = 1;tile <=2000;tile++)
    {
        for(int cycle = 1;cycle <=318;cycle++)
        {

            extraction_metric metric = extraction_metric_set.GetMetric(lane, tile, cycle);
            sum += metric.focusScore(0);
        }
    }
}

```

### 1. Indirect Call

One loop in C++
One loop in C#

```csharp
extraction_metric_set.copy_focus(focusVals, 0, (uint)focusVals.Length);
double sum = 0.0;
for(int i=0;i<focusVals.Length;i++) sum+=focusVals[i];
```

### Results

The following table summarizes the results (the times are a single sample for illustrative purposes)

Test Name     | Time (s)
------------- | -----------
Direct Call 1 | 38
Direct Call 2 | 45
Indirect Call | < 1

The results show that everytime we make a call from the native language to C++, we pay a hefty price. In many applications
this is a price we are willing to pay. However, for those applications were performance is a concern, it pays to add
methods like `copy_focus`, which limit the number of times we have to jump between C# (or another language) and C++.