/*

    SHUFFLE COMPANY SOURCE CODE

    sc_stage.cpp
    ------------------------------------



*/

#include "sc_stage.h"

namespace sc
{
	Brush::Brush(std::string mat)
	{
		mat_E = mat;
		mat_N = mat;
		mat_W = mat;
		mat_S = mat;
		mat_T = mat;
		mat_B = mat;		
	}

	bool Stage::loadStage(std::string filepath)
	{
		std::ifstream file;
		file.open(filepath);

		if (file)
		{
			sc::Tokenizer t(&file);
			t.addDelimiter('\r');
			t.addDelimiter('\n');
			t.addDelimiter(' ');

			t.next();

			if (t.check("word", "shuff"))
			{
				LOG_I << "Reading shuff file";
				t.next();
				if (t.check("word", "brushes"))
				{
					LOG_I << "Reading brushes";
					t.next();
					if (t.checkType("word"))
					{
						while (t.checkType("word") && !t.checkToken("stage"))
						{
							brushes.push_back(Brush(t.getToken()));
							LOG_I << "Material: " << t.getToken();
							t.next();
							if (t.checkType("statementEnd"))
							{
								LOG_I << "End Brush";
							}
							else break;
							t.next();
						}

						if (t.check("word", "stage"))
						{
							LOG_I << "Reading stage";
							t.next();
							if (t.checkType("int"))
							{
								while (t.checkType("int"))
								{
									LOG_I << "Brush: " << t.getToken();
									t.next();
									if (t.checkType("int"))
									{
										LOG_I << "X: " << t.getToken();
										t.next();
										if (t.checkType("int"))
										{
											LOG_I << "Y: " << t.getToken();
											t.next();
											if (t.checkType("int"))
											{
												LOG_I << "Z: " << t.getToken();
												t.next();
												if (t.checkType("statementEnd"))
												{
													LOG_I << "End Stage Chunk";
													if (!t.next())
													{
														file.close();
														LOG_I << "Read shuff file successfully";
														return true;
													}											
												}
												else break;
											}
											else break;
										}
										else break;
									}
									else break;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			LOG_E << "Could not open shuff file " << filepath;
			return false;
		}

		file.close();
		LOG_E << "Invalid shuff file data";
		return false;
	}

	sc::Model* Stage::getModel()
	{
		return model;
	}
}