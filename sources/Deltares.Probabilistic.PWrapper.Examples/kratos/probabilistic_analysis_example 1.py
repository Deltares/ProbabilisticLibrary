import os
import tempfile
import shutil
import numpy as np

import KratosMultiphysics as Kratos
import KratosMultiphysics.GeoMechanicsApplication as kratos_geo
import KratosMultiphysics.StructuralMechanicsApplication as kratos_struct

import KratosMultiphysics.GeoMechanicsApplication.geomechanics_analysis as analysis
import KratosMultiphysics.GeoMechanicsApplication.prob_analysis as prob_analysis
import KratosMultiphysics.GeoMechanicsApplication.test_helper as test_helper

import matplotlib.pyplot as plt

if __name__ == "__main__":
    young_moduli = [100e5 * i for i in range(1,15)]
    max_bending_moment = []
    for young_modulus in young_moduli:
        template_project_path = os.path.join(__file__, os.pardir, "Quay_Wall_4Stage_with_mohr_coulomb")
        #template_project_path = r"c:\Werk\kratos\QuayWallModel_new"
        stage_number=1
        number_of_soil_parts = 7
        input_parameters = [[stage_number, "PorousDomain.Parts_Solid_layer_1|1", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_1|2", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_2|1", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_2|2", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_3|1", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_3|2", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Solid_layer_4|1", Kratos.YOUNG_MODULUS],
                            [stage_number, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.CROSS_AREA],
                            [stage_number, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.AREA_EFFECTIVE_Y],
                            [stage_number, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.I33]]
        input_values = [young_modulus] * number_of_soil_parts + [0.025, 0.025, 5.208E-4]
        # Format of a single 'output_parameters'  entry = 
        # [<stage_nr>, <function you want to perform on the results (can be None)>, <get-function>, <ModelPartName>, <VariableName>, <node_id (can be None>)]
        output_parameters = [[3, np.max, test_helper.get_on_integration_points, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.BENDING_MOMENT],
                             [3, None, test_helper.get_on_integration_points, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.BENDING_MOMENT, [410, 412]]]

        try:
            prob_analysis_instance = prob_analysis.prob_analysis(template_project_path, input_parameters, output_parameters)
            output_values = prob_analysis_instance.calculate(input_values)
            max_bending_moment.append(output_values[0])
        except Exception as e:
            print("An error occurred:", e)
        finally:
            # Clean up and remove the temporary folder
            prob_analysis_instance.finalize()

    plt.plot(young_moduli, max_bending_moment)
    plt.xlabel("Young's Modulus of the entire soil (Pa)")
    plt.ylabel("Max bending moment in the sheetpile")
    plt.show()