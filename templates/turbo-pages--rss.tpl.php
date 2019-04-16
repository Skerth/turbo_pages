
<rss xmlns:turbo="http://turbo.yandex.ru" version="2.0">
  <channel>
    <title><?php print $channel_title; ?></title>
    <link>https://dovir-finance.ru/</link>
    <description><?php print $channel_description; ?></description>
    <language>ru</language>

    <?php foreach ($nodes as $node): ?>
      <item turbo="true">
        <link><?php print 'https://dovir-finance.ru' . url('node/' . $node->nid/*, array('absolute' => TRUE)*/); ?></link>
        <pubDate><?php print date(DATE_RFC822, $node->created); ?></pubDate>
        <author><?php print $node->$username_field; ?></author>
        <turbo:content>
          <![CDATA[
          <header>
             <h1><?php print $node->title; ?></h1>
          </header>
          <?php print $node->body_value; ?>
          ]]>
        </turbo:content>
      </item>
    <?php endforeach; ?>

  </channel>
</rss>
