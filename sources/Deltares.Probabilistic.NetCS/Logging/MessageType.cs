using System;

namespace Deltares.Probabilistic.Logging;

public enum MessageType
{
    Info,
    Debug,
    Warning,
    Error
};

public static class MessageTypeConverter
{
    public static string ConvertToString(MessageType type)
    {
        return type switch
        {
            MessageType.Info => "info",
            MessageType.Debug => "debug",
            MessageType.Warning => "warning",
            MessageType.Error => "error",
            _ => throw new ArgumentOutOfRangeException(nameof(type), type, null)
        };
    }

    public static MessageType ConvertFromString(string value)
    {
        return value.ToLowerInvariant() switch
        {
            "info" => MessageType.Info,
            "debug" => MessageType.Debug,
            "warning" => MessageType.Warning,
            "error" => MessageType.Error,
            _ => throw new ArgumentException($"Unknown message type: {value}")
        };
    }
}
