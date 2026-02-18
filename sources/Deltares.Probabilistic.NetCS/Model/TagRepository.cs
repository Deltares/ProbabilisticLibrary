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
        tags[++counter] = tag;
        return counter;
    }

    public object RetrieveTag(int index) 
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