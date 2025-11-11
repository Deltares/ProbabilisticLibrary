FROM mcr.microsoft.com/dotnet/runtime:8.0
WORKDIR /app

RUN apt-get update && \
    apt-get install -y --no-install-recommends libgomp1 libc6 && \
    rm -rf /var/lib/apt/lists/*

COPY . .
# copy:
# Deltares.Examples.dll
# Deltares.Probabilistic.Interface.dll
# libDeltares.Probabilistic.CWrapper.so
# Deltares.Examples.runtimeconfig.json

ENTRYPOINT ["dotnet", "Deltares.Examples.dll"]
