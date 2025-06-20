import numpy as np
import os

import KratosMultiphysics as Kratos
import KratosMultiphysics.GeoMechanicsApplication as kratos_geo
import KratosMultiphysics.StructuralMechanicsApplication as kratos_struct

import KratosMultiphysics.GeoMechanicsApplication.prob_analysis as prob_analysis
import KratosMultiphysics.GeoMechanicsApplication.test_helper as test_helper

class zfuncwrapper_linear:

    def __init__(self, output_stage_number, clean_up):

        self.output_stage_number = output_stage_number
        self.clean_up = clean_up

        self.template_project_path = os.path.join(__file__, os.pardir, "QuayWallModel")

        stage_numbers = [0] # the wrapper of Kratos applies these values to all stages, so we can just use one stage number

        layers = ["PorousDomain.Parts_Solid_layer_1|1",
                  "PorousDomain.Parts_Solid_layer_1|2",
                  "PorousDomain.Parts_Solid_layer_2|1",
                  "PorousDomain.Parts_Solid_layer_2|2",
                  "PorousDomain.Parts_Solid_layer_3|1",
                  "PorousDomain.Parts_Solid_layer_3|2",
                  "PorousDomain.Parts_Solid_layer_4|1"]
        
        interfaces = ["PorousDomain.Layer11Interface",
                      "PorousDomain.Layer12Interface",
                      "PorousDomain.Layer21Interface",
                      "PorousDomain.Layer22Interface",
                      "PorousDomain.Layer31Interface",
                      "PorousDomain.Layer32Interface"]
        
        interfaces_variables = [kratos_geo.INTERFACE_NORMAL_STIFFNESS]
        
        layers_variables = [Kratos.YOUNG_MODULUS]
        
        sheetpile_variables = [Kratos.YOUNG_MODULUS, 
                               Kratos.POISSON_RATIO, 
                               kratos_struct.CROSS_AREA, 
                               kratos_struct.AREA_EFFECTIVE_Y, 
                               kratos_struct.I33]
        
        self.input_parameters = []
        for variable in layers_variables:
            for stage_number in stage_numbers:
                for layer in layers:
                    self.input_parameters.append([stage_number, layer, variable])

        for variable in interfaces_variables:
            for stage_number in stage_numbers:
                for interface in interfaces:
                    self.input_parameters.append([stage_number, interface, variable])

        for variable in sheetpile_variables:
            for stage_number in stage_numbers:
                self.input_parameters.append([stage_number, "PorousDomain.Parts_Beam_sheetpile", variable])

    def _expand_young_modulus(self, geo_young_modulus_in: list[float]) -> list[float]:
        # if only two values are provided, the first is for the 5 clay layers, the second the 2 sand layers:
        # otherwise all layers are independent
        if (len(geo_young_modulus_in) == 2):
            geo_young_modulus = []
            for i in range(4):
                geo_young_modulus.append(geo_young_modulus_in[0])
            for i in range(3):
                geo_young_modulus.append(geo_young_modulus_in[1])
        else:
            geo_young_modulus = geo_young_modulus_in
        return geo_young_modulus

    def total_displacement(self, 
                           geo_young_modulus: list[float],
                           interface_stiffness: list[float],
                           sheetpile_young_modulus: list[float],
                           sheetpile_poisson_ratio: list[float],
                           sheetpile_thickness: list[float]) -> float:

        thickness = sheetpile_thickness + sheetpile_thickness + [val**3/12 for val in sheetpile_thickness]
        
        geo_young_modulus_full = self._expand_young_modulus(geo_young_modulus)
            
        input_list = geo_young_modulus_full + interface_stiffness + sheetpile_young_modulus + sheetpile_poisson_ratio + thickness
        
        # Format of a single 'output_parameters'  entry = 
        # [<stage_nr>, <function you want to perform on the results (can be None)>, <get-function>, <ModelPartName>, <VariableName>, <node_id (can be None>)]
        output_parameters = [[self.output_stage_number, None, test_helper.get_nodal_variable, "PorousDomain", kratos_geo.TOTAL_DISPLACEMENT_X, 171]]
        prob_analysis_instance = prob_analysis.prob_analysis(self.template_project_path, self.input_parameters, output_parameters)
        output_values = prob_analysis_instance.calculate(input_list)
        total_displacement = output_values[0]
        
        if self.clean_up:
            prob_analysis_instance.finalize()

        return total_displacement

    def max_bending_moment(self, 
                           geo_young_modulus: list[float], 
                           interface_stiffness: list[float],
                           sheetpile_young_modulus: list[float],
                           sheetpile_poisson_ratio: list[float],
                           sheetpile_thickness: list[float]) -> float:
        
        thickness = sheetpile_thickness + sheetpile_thickness + [val**3/12 for val in sheetpile_thickness]

        geo_young_modulus_full = self._expand_young_modulus(geo_young_modulus)

        input_list = geo_young_modulus_full + interface_stiffness + sheetpile_young_modulus + sheetpile_poisson_ratio + thickness

        # Format of a single 'output_parameters'  entry = 
        # [<stage_nr>, <function you want to perform on the results (can be None)>, <get-function>, <ModelPartName>, <VariableName>, <node_id (can be None>)]
        output_parameters = [[self.output_stage_number, np.max, test_helper.get_on_integration_points, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.BENDING_MOMENT],
                             [self.output_stage_number, None, test_helper.get_on_integration_points, "PorousDomain.Parts_Beam_sheetpile", kratos_struct.BENDING_MOMENT, [410, 412]]]

        prob_analysis_instance = prob_analysis.prob_analysis(self.template_project_path, self.input_parameters, output_parameters)
        output_values = prob_analysis_instance.calculate(input_list)
        max_bending_moment = output_values[0]

        if self.clean_up:
            prob_analysis_instance.finalize()

        return max_bending_moment

