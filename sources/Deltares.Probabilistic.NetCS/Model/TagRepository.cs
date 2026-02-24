using System.Collections.Generic;

namespace Deltares.Probabilistic.Model;

public class TagRepository
{
    private Dictionary<int, object> tags = new Dictionary<int, object>();
    private int counter = 0;
    private readonly object lockObject = new object();

    public TagRepository()
    {
    }

    public int RegisterTag(object tag)
    {
        if (tag != null)
        {
            lock (lockObject)
            {
                tags[++counter] = tag;
                return counter;
            }
        }
        else
        {
            return 0;
        }
    }

    public object RetrieveTag(int index) 
    {
        if (index == 0)
        {
            return null;
        }
        else
        {
            lock (lockObject)
            {
                if (tags.ContainsKey(index))
                {
                    return tags[index];
                }
                else
                {
                    return null;
                }
            }
        }
    }
}