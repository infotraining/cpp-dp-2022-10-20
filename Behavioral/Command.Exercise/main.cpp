#include <iostream>

#include "application.hpp"
#include "command.hpp"
#include <boost/di.hpp>  // https://github.com/boost-ext/di

using namespace std;
namespace di = boost::di;

int main()
{
    Document doc;

    const auto injector = di::make_injector(
        di::bind<Console>().to<Terminal>(),
        di::bind<Clipboard>().to<SharedClipboard>(),
        di::bind<Document>().to(doc));


    auto app = injector.create<Application>();

    // Terminal term;
    // Document doc;
    // Application app(term);

    // auto print_cmd = std::make_shared<PrintCmd>(doc, term);
    // app.add_command("Print", print_cmd);

    app.add_command("Print"s, injector.create<std::shared_ptr<PrintCmd>>());
    app.add_command("ToUpper"s, injector.create<std::shared_ptr<ToUpperCmd>>());
    app.add_command("Clear"s, injector.create<std::shared_ptr<ClearCmd>>());
    app.add_command("AddText"s, injector.create<std::shared_ptr<AddTextCmd>>());
    app.add_command("Paste"s, injector.create<std::shared_ptr<PasteCmd>>());
    app.add_command("Undo"s, injector.create<std::shared_ptr<UndoCmd>>());

    // TODO - register two commands: CopyCmd & ToLowerCmd

    app.run();
}
