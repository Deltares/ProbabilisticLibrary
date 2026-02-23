using System.Collections.Generic;

namespace Deltares.Probabilistic.Model;

public class TagRepository
{
    private Dictionary<int, object> tags = new Dictionary<int, object>();
    private int counter = 0;

    public TagRepository()
    {
    }

    public int RegisterTag(object tag)
    {
        if (tag != null)
        {
            tags[++counter] = tag;
            return counter;
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
        else if (tags.ContainsKey(index))
        {
            return tags[index];
        }
        else
        {
            return null;
        }
    }
}