import KratosMultiphysics as Kratos

from KratosMultiphysics.GeoMechanicsApplication.geomechanics_analysis import GeoMechanicsAnalysis

if __name__ == "__main__":
    # construct parameterfile names of stages to run
    n_stages = 4
    parameter_file_names = [f'ProjectParameters_Stage{i+1}.json' for i in range(n_stages)]

    # setup stages from parameterfiles
    parameters_stages = [None] * n_stages
    for idx, parameter_file_name in enumerate(parameter_file_names):
        with open(parameter_file_name, 'r') as parameter_file:
            parameters_stages[idx] = Kratos.Parameters(parameter_file.read())

    model = Kratos.Model()
    for stage_parameters in parameters_stages:
        stage = GeoMechanicsAnalysis(model, stage_parameters)
        stage.Run()
