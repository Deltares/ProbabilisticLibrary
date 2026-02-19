using Deltares.Probabilistic.Utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Deltares.Probabilistic.Logging
{
    public delegate void ProgressDelegate(double progress);
    public delegate void DetailedProgressDelegate(int step, int loop, double reliability, double convergence);
    public delegate void TextualProgressDelegate(ProgressType progressType, string text);

    public class ProgressIndicator : IDisposable
    {
        private ProgressDelegate progressDelegate = null;
        private DetailedProgressDelegate detailedProgressDelegate = null;
        private TextualProgressDelegate textualProgressDelegate = null;

        private int id = 0;

        public ProgressIndicator(ProgressDelegate progressDelegate, DetailedProgressDelegate detailedProgressDelegate, TextualProgressDelegate textualProgressDelegate)
        {
            this.id = Interface.Create("progress_indicator");

            this.progressDelegate = progressDelegate;
            this.detailedProgressDelegate = detailedProgressDelegate;
            this.textualProgressDelegate = textualProgressDelegate;
        }

        public void Dispose()
        {
            Interface.Destroy(id);
        }

        internal int GetId()
        {
            return id;
        }

        public void DoProgress(double progress)
        {
            progressDelegate?.Invoke(progress);
        }

        public void DoDetailedProgress(int step, int loop, double reliability, double convergence)
        {
            detailedProgressDelegate?.Invoke(step, loop, reliability, convergence);
        }

        public void DoTextualProgress(ProgressType progressType, string text)
        {
            textualProgressDelegate?.Invoke(progressType, text);
        }
    }
}
