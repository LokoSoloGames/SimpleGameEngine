#include "ShaderCompiler_DX11.h"
#include "ShaderParser.h"

namespace SimpleGameEngine {

	class ShaderCompiler : public ConsoleApp {
	public:

	protected:
		virtual void onRun() {
			{
				String file = getExecutableFilename();
				String path = FilePath::dirname(file);
				path.append("/../../../app/assets");
				Directory::setCurrent(path);
			}

			compile("Shaders/test.shader");
			compile("Shaders/test2.shader");
			compile("Shaders/terrain.shader");
		}

		void compile(StrView shaderFilename) {
			ShaderInfo info;

			String outputPath = Fmt("LocalTemp/Imported/{}", shaderFilename);
			Directory::create(outputPath);

			TempString code;
			auto codeFilename = Fmt("{}/code.hlsl", outputPath);
			{
				ShaderParser parser;
				parser.readFile(info, shaderFilename);

				auto jsonFilename = Fmt("{}/info.json", outputPath);
				JsonUtil::writeFile(jsonFilename, info, false);

				for (size_t i = 2; i < parser.line(); i++) {
					code += "//\n";
				}
				auto remain = parser.getRemainSource();
				code += remain;

				File::writeFileIfChanged(codeFilename, code, true);
			}

			{ // DX11
				size_t passIndex = 0;
				for (auto& pass : info.passes) {
					auto passOutPath = Fmt("{}/dx11/pass{}", outputPath, passIndex);

					if (pass.vsFunc.size()) {
						ShaderCompiler_DX11 c;
						c.compile(passOutPath, ShaderStageMask::Vertex, codeFilename, pass.vsFunc);
					}

					if (pass.psFunc.size()) {
						ShaderCompiler_DX11 c;
						c.compile(passOutPath, ShaderStageMask::Pixel, codeFilename, pass.psFunc);
					}

					passIndex++;
				}
			}

			SGE_LOG("---- end ----");
		}

	};

}

int main() {
	SimpleGameEngine::ShaderCompiler app;
	app.run();

	return 0;
}