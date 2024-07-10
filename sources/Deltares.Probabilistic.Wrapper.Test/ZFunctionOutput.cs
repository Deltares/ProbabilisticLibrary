namespace Deltares.Probabilistic.Wrapper.Test
{
    public delegate ZFunctionOutput ZDelegate(params double[] x);

    /// <summary>
    /// Class containing the output of a Z function call.
    /// </summary>
    public class ZFunctionOutput
    {
        /// <summary>
        /// Creates a new instance of <see cref="ZFunctionOutput"/>.
        /// </summary>
        /// <param name="z">The z value.</param>
        public ZFunctionOutput(double z)
        {
            Z = z;
        }

        /// <summary>
        /// Gets or sets the Z value returned by the Z function
        /// </summary>
        public double Z { get; }
    }
}