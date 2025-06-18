import os
import tempfile
import shutil
import numpy as np

import KratosMultiphysics as Kratos
import KratosMultiphysics.GeoMechanicsApplication as kratos_geo
import KratosMultiphysics.GeoMechanicsApplication.geomechanics_analysis as analysis
import KratosMultiphysics.GeoMechanicsApplication.prob_analysis as prob_analysis
import KratosMultiphysics.GeoMechanicsApplication.test_helper as test_helper

import matplotlib.pyplot as plt

if __name__ == "__main__":
    young_moduli = [100e5, 200e5, 500e5, 1000e5, 1500e5]
    max_y_displacement = []
    for young_modulus in young_moduli:
        template_project_path = os.path.join(__file__, os.pardir, "Quay_Wall_4Stage_with_mohr_coulomb")
        #template_project_path = r"c:\Werk\kratos\QuayWallModel_new"
        stage_number=1
        input_parameters = [[stage_number, "PorousDomain.Parts_Solid_layer_1|1", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_1|2", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_2|1", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_2|2", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_3|1", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_3|2", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_4|1", Kratos.YOUNG_MODULUS]]
        input_values = [young_modulus] * len(input_parameters)
        # Format of a single 'output_parameters'  entry = 
        # [<stage_nr>, <function you want to perform on the results (can be None)>, <get-function>, <ModelPartName>, <VariableName>, <node_id (can be None>)]
        output_parameters = [[3, np.min, test_helper.get_nodal_variable, "PorousDomain", Kratos.DISPLACEMENT_Y, None]]

        prob_analysis_instance = prob_analysis.prob_analysis(template_project_path, input_parameters, output_parameters)
        output_values = prob_analysis_instance.calculate(input_values)
        max_y_displacement.append(output_values[0])

        #try:
        #    prob_analysis_instance = prob_analysis.prob_analysis(template_project_path, input_parameters, output_parameters)
        #    output_values = prob_analysis_instance.calculate(input_values)
        #    max_y_displacement.append(output_values[0])
        #except Exception as e:
        #    print("An error occurred:", e)
        #finally:
            # Clean up and remove the temporary folder
        #    prob_analysis_instance.finalize()

    plt.plot(young_moduli, max_y_displacement)
    plt.xlabel("Young's Modulus of the entire soil (Pa)")
    plt.ylabel("Max negative Y Displacement (m)")
    plt.show()
