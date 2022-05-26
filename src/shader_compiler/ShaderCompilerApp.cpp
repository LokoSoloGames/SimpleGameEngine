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

			ShaderInfo info;

			StrView shaderFilename = "Shaders/test.shader";

			String outputPath = Fmt("LocalTemp/Imported/{}", shaderFilename);
			Directory::create(outputPath);

			TempString code;
			auto codeFilename = Fmt("{}/code.hlsl", outputPath);
			{
				ShaderParser parser;
				parser.readFile(info, shaderFilename);

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
						ShaderLayout layout;
						ShaderCompiler_DX11 c;
						c.compile(passOutPath, ShaderStage::Vertex, codeFilename, pass.vsFunc, layout);
					}

					if (pass.psFunc.size()) {
						ShaderLayout layout;
						ShaderCompiler_DX11 c;
						c.compile(passOutPath, ShaderStage::Pixel, codeFilename, pass.psFunc, layout);
					}

					passIndex++;
				}
			}

			SGE_LOG("---- end ----");
		}

		void compile(StrView filename) {
		}

	};

}

int main() {
	SimpleGameEngine::ShaderCompiler app;
	app.run();

	return 0;
}