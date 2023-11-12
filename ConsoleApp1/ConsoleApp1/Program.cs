using Microsoft.Office.Interop.PowerPoint;
using System;
using Microsoft.Office.Core;
using System.Threading;


class Program
{
    static void Main()
    {
        // Start PowerPoint and open the presentation.
        Application pptApplication = new Application();
        Presentation presentation = pptApplication.Presentations.Open(@"E:\Course\malware\test.pptm", WithWindow: MsoTriState.msoTrue);
        // Run the macro.
        // Replace "MacroName" with the actual name of the macro you want to run.
        Thread.Sleep(5000);
        pptApplication.Run("init");

        // Clean up.
        presentation.Close();
        pptApplication.Quit();
    }
}
