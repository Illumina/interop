using System;
using NUnit.Framework;
using Illumina.InterOp.RunMetrics;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Test C# dependencies
	/// </summary>
	public class DependencyTests
	{
		/// <summary>
		/// Test FileNotFoundException
		/// </summary>
		[Test]
		public void TestFileNotFoundException()
		{
            run_metrics metrics = new run_metrics();
            try{
                metrics.read("/NO/FILE/EXISTS");
            }
            catch(Exception){}
		}
	}
}
