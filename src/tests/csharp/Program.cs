using System;
using NUnit.Framework;
using NUnitLite;
using System.Reflection;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Test C# Swig Wrapping
	/// </summary>
	public class Program
	{
        public static int Main(string[] args)
        {
           return new AutoRun(typeof(CoreTests).GetTypeInfo().Assembly).Execute(args);
        }
    }
}