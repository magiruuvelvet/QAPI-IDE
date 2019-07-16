#ifndef UI_MENU_CREATOR_HPP
#define UI_MENU_CREATOR_HPP

#include <memory>
#include <list>
#include <functional>

#include <QString>
#include <QMenu>
#include <QAction>

struct menu_entry;

struct menu final
{
    std::shared_ptr<QMenu> menu;
    std::list<menu_entry> actions;
};

struct menu_entry final
{
public:
    // separator
    menu_entry()
    {
        entry = std::make_shared<QAction>();
        entry->setSeparator(true);
    }

    // sub menu
    menu_entry(const menu &submenu)
    {
        entry = std::make_shared<QAction>(submenu.menu->title());
        entry->setMenu(submenu.menu.get());
    }
    menu_entry(const menu *submenu)
    {
        entry = std::make_shared<QAction>(submenu->menu->title());
        entry->setMenu(submenu->menu.get());
    }

    // real entry
    menu_entry(const QString &name, const QKeySequence &shortcut,
               const QWidget *receiver, const std::function<void()> &callback,
               bool enabled = true)
    {
        entry = std::make_shared<QAction>(name);
        entry->setEnabled(enabled);
        entry->setShortcut(shortcut);
        QObject::connect(entry.get(), &QAction::triggered, receiver, callback);
    }

    inline constexpr QAction *get()
    {
        return entry.get();
    }

private:
    std::shared_ptr<QAction> entry;
};

struct menu_creator final
{
public:
    template<typename... Args>
    static std::shared_ptr<menu> make_menu(const QString &name, Args... args)
    {
        auto menu_decl = std::make_shared<menu>();
        menu_decl->menu = std::make_shared<QMenu>(name);
        menu_decl->actions = {args...};

        for (auto&& action : menu_decl->actions)
        {
            menu_decl->menu->addAction(action.get());
        }

        return menu_decl;
    }

private:
    menu_creator() = delete;
};

#endif // UI_MENU_CREATOR_HPP
